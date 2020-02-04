/*
* Name:		cPhysicsWorld.h
* Author:	Daniel Maclam
* Date		2019-02-01 2:01 PM
*/

#include "cPhysicsWorld.h"
#include "physics/interfaces/iRigidBody.h"
#include "shapes/cPlane.h"

int physics::cPhysicsWorld::intersect_moving_sphere_plane(interfaces::iRigidBody* s, glm::vec3 movement_vector,
	interfaces::iRigidBody* p, float& t, glm::vec3& q) const
{
	//get the shapes
	interfaces::shapes::iSphere* sphere = static_cast<interfaces::shapes::iSphere*>(s->get_shape());
	interfaces::shapes::iPlane* plane = static_cast<interfaces::shapes::iPlane*>(p->get_shape());

	// Compute distance of sphere center to plane
	float dist = glm::dot(plane->get_normal(), s->get_position()) - plane->get_constant();
	if (glm::abs(dist) <= sphere->get_radius()) {
		// The sphere is already overlapping the plane. Set time of
		// intersection to zero and q to sphere center
		t = 0.0f;
		q = s->get_position();
		return -1; //(changed this so we know we are already colliding
	}

	float denom = glm::dot(plane->get_normal(), (glm::normalize(s->get_velocity())));
	if (denom * dist >= 0.0f) {
		// No intersection as sphere moving parallel to or away from plane
		return 0;
	}
	else {
		// Sphere is moving towards the plane
		// Use +r in computations if sphere in front of plane, else -r
		float r = dist > 0.0f ? sphere->get_radius() : -sphere->get_radius();
		t = (r - dist) / denom;
		q = s->get_position() + t * s->get_velocity() - r * plane->get_normal();
		return 1;
	}
}

int physics::cPhysicsWorld::intersect_moving_sphere_sphere(interfaces::iRigidBody* s1, interfaces::iRigidBody* s2, float& t) const
{

	interfaces::shapes::iSphere* sphere1 = dynamic_cast<interfaces::shapes::iSphere*>(s1->get_shape());
	interfaces::shapes::iSphere* sphere2 = dynamic_cast<interfaces::shapes::iSphere*>(s2->get_shape());

	glm::vec3 s = s2->get_position() - s1->get_position();
	glm::vec3 v = glm::normalize(s2->get_velocity() - s1->get_velocity());
	float r = sphere1->get_radius() + sphere2->get_radius();
	float c = glm::dot(s, s) - r * r;

	if (c < 0.0f)
	{
		//spheres are overlapping so exit
		t = 0.0f;
		return -1;
	}

	float a = glm::dot(v, v);
	if (a < glm::epsilon<float>())
	{
		//not moving relative to each other
		return 0;
	}
	float b = glm::dot(v, s);
	if (b >= 0.0f)
	{
		//moving away from each other
		return 0;
	}

	float d = b * b - a * c;
	if (d < 0.0f)
	{
		//they do not intersect
		return 0;
	}

	t = (-b - glm::sqrt(d)) / a;
	return 1;
}

physics::cPhysicsWorld::Derivative physics::cPhysicsWorld::evaluate(const State& initial, glm::vec3 t, float dt,
	const Derivative& d)
{
	State state;
	state.p = initial.p + d.dx*dt;
	state.v = initial.v + d.dv*dt;

	Derivative output;
	output.dx = state.v;
	output.dv += t;

	return output;
}

void physics::cPhysicsWorld::integrate(State& state, glm::vec3 t, float dt)
{
	Derivative a, b, c, d;

	a = evaluate(state, t, 0.0f, Derivative());
	b = evaluate(state, t, dt*0.5f, a);
	c = evaluate(state, t, dt*0.5f, b);
	d = evaluate(state, t, dt, c);

	glm::vec3 dxdt = 1.0f / 6.0f *	(a.dx + 2.0f * (b.dx + c.dx) + d.dx);

	glm::vec3 dvdt = 1.0f / 6.0f * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);

	state.p = state.p + dxdt * dt;
	state.v = state.v + dvdt * dt;
}

void physics::cPhysicsWorld::register_rigid_body(interfaces::iRigidBody* body)
{
	this->rigid_bodies.push_back(body);
}

void physics::cPhysicsWorld::unregister_rigid_body(interfaces::iRigidBody* body)
{
	for (size_t i = 0; i < this->rigid_bodies.size() - 1; ++i)
	{
		if (this->rigid_bodies[i] == body)
		{
			this->rigid_bodies.erase(this->rigid_bodies.begin() + i);
			break;
		}
	}
}

struct collision_data
{
	enum eCollisionType
	{
		SPHERE_PLANE,
		SPHERE_SPHERE,
	};

	collision_data() : contactVelocity(glm::vec3(0.0f)), contactVelocity_other(glm::vec3(0.0f)), contactXYZ(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), penetrationDepth(0.0f),
		entityA(nullptr),
		entityB(nullptr),
		time_step(0.0f), collisionType(SPHERE_PLANE)
	{
	}

	glm::vec3 contactVelocity;
	glm::vec3 contactVelocity_other;

	glm::vec3 contactXYZ;
	glm::vec3 normal;
	float penetrationDepth;

	physics::interfaces::iRigidBody* entityA;
	physics::interfaces::iRigidBody* entityB;

	float time_step;

	eCollisionType collisionType;

};

void physics::cPhysicsWorld::update(float delta_time)
{
	total_time += delta_time;

	for (size_t i = 0; i < this->rigid_bodies.size(); ++i)
	{
		interfaces::iRigidBody* body = this->rigid_bodies[i];

		//integrate spheres
		if (body->get_shape_type() == interfaces::iShape::sphere)
		{
			//for RK4
			State objectState;
			objectState.p = body->get_position();
			objectState.v = body->get_velocity();

			body->apply_force(glm::vec3(0.0f, -10.0f, 0.0f));

			integrate(objectState, body->get_acceleration(), delta_time);

			body->set_velocity(objectState.v);
			body->set_position(objectState.p);

			body->set_acceleration(glm::vec3(0.0f));
		}

		//store the last position
		body->set_previous_position(body->get_position());
	}

	//collision detect
	std::vector<collision_data> collisions;
	for (size_t i = 0; i < this->rigid_bodies.size() - 1; ++i)
	{
		interfaces::iRigidBody* body = this->rigid_bodies[i];

		for (size_t j = i + 1; j < this->rigid_bodies.size(); ++j)
		{
			float time_step;
			glm::vec3 q;

			//plane/sphere collision
			if (this->rigid_bodies[i]->get_shape_type() == interfaces::iShape::plane && this->rigid_bodies[j]->get_shape_type() == interfaces::iShape::sphere)
			{
				int result = intersect_moving_sphere_plane(this->rigid_bodies[j], this->rigid_bodies[j]->get_velocity(), this->rigid_bodies[i], time_step, q);

				if (result == -1)
				{
					//set the sphere to it's previous position and out the normal a bit
					glm::vec3 normal = static_cast<interfaces::shapes::iPlane*>(this->rigid_bodies[i]->get_shape())->get_normal();

					//move out of the collision
					glm::vec3 newPos = this->rigid_bodies[j]->get_position() + (normal * delta_time);
					newPos.y += static_cast<interfaces::shapes::iSphere*>(this->rigid_bodies[j]->get_shape())->get_radius();
					this->rigid_bodies[j]->set_position(newPos);
				}

				if (result == 1)
				{
					//new collision
					if (time_step > 0.0f && time_step < 1.0f)
					{
						interfaces::shapes::iPlane* plane = static_cast<interfaces::shapes::iPlane*>(this->rigid_bodies[i]->get_shape());

						collision_data cd;
						cd.collisionType = collision_data::SPHERE_PLANE;
						cd.contactXYZ = q;
						cd.entityA = this->rigid_bodies[j];
						cd.entityB = this->rigid_bodies[i];
						cd.contactVelocity = this->rigid_bodies[j]->get_velocity();
						cd.contactVelocity_other = this->rigid_bodies[i]->get_velocity();
						cd.normal = plane->get_normal();
						cd.time_step = time_step;
						collisions.push_back(cd);
					}
				}
			}

			if (this->rigid_bodies[j]->get_shape_type() == interfaces::iShape::plane && this->rigid_bodies[i]->get_shape_type() == interfaces::iShape::sphere)
			{
				time_step = 0.0f;
				int result = intersect_moving_sphere_plane(this->rigid_bodies[i], this->rigid_bodies[i]->get_velocity(), this->rigid_bodies[j], time_step, q);

				if (result == -1)
				{
					//set the sphere to it's previous position and out the normal a bit
					glm::vec3 normal = static_cast<interfaces::shapes::iPlane*>(this->rigid_bodies[j]->get_shape())->get_normal();

					//move out of the collision
					glm::vec3 newPos = this->rigid_bodies[i]->get_position() + (normal * delta_time);
					newPos.y += static_cast<interfaces::shapes::iSphere*>(this->rigid_bodies[j]->get_shape())->get_radius();
					this->rigid_bodies[i]->set_position(newPos);
				}

				if (result == 1)
				{
					if (time_step > 0.0f && time_step < 1.0f)
					{
						interfaces::shapes::iPlane* plane = static_cast<interfaces::shapes::iPlane*>(this->rigid_bodies[j]->get_shape());

						collision_data cd;
						cd.collisionType = collision_data::SPHERE_PLANE;
						cd.contactXYZ = q;

						cd.entityA = this->rigid_bodies[i];
						cd.entityB = this->rigid_bodies[j];
						cd.contactVelocity = this->rigid_bodies[i]->get_velocity();
						cd.contactVelocity_other = this->rigid_bodies[j]->get_velocity();
						cd.normal = plane->get_normal();
						cd.time_step = time_step;

						collisions.push_back(cd);
					}
				}
			}

			//sphere / sphere
			if (this->rigid_bodies[j]->get_shape_type() == interfaces::iShape::sphere && this->rigid_bodies[i]->get_shape_type() == interfaces::iShape::sphere)
			{
				glm::vec3 contact_point(0.0f);
				time_step = 0.0f;
				int result = intersect_moving_sphere_sphere(this->rigid_bodies[j], this->rigid_bodies[i], time_step);

				if (result == -1)
				{
					this->rigid_bodies[i]->set_position(this->rigid_bodies[i]->get_previous_position());
					this->rigid_bodies[j]->set_position(this->rigid_bodies[j]->get_previous_position());
				}

				if (result == 1)
				{
					if (time_step > 0.0f && time_step < 1.0f)
					{
						collision_data cd;
						cd.collisionType = collision_data::SPHERE_SPHERE;
						cd.entityA = this->rigid_bodies[j];
						cd.entityB = this->rigid_bodies[i];
						cd.contactVelocity = this->rigid_bodies[j]->get_velocity();
						cd.contactVelocity_other = this->rigid_bodies[i]->get_velocity();
						cd.normal = glm::normalize(this->rigid_bodies[j]->get_position() - this->rigid_bodies[i]->get_position());
						cd.time_step = time_step;

						collisions.push_back(cd);
					}
				}
			}
		}
	}

	//collision response
	for (size_t i = 0; i < collisions.size(); ++i)
	{
		float velocity_decay = 0.10f; // decay percent

		if (collisions[i].collisionType == collision_data::SPHERE_PLANE)
		{
			float time_no_collision = (delta_time * collisions[i].time_step);
			float time_after_collision = delta_time - time_no_collision;

			//reflect velocity using plane normal
			glm::vec3 reflectedVelocity = glm::reflect(collisions[i].contactVelocity, collisions[i].normal);

			collisions[i].entityA->set_velocity(reflectedVelocity);

			//simulate the rest of the timestep
			State newStateA;
			newStateA.p = collisions[i].entityA->get_position();
			newStateA.v = collisions[i].entityA->get_velocity();

			integrate(newStateA, glm::vec3(0.0f), time_after_collision);
			collisions[i].entityA->set_velocity(newStateA.v);
			collisions[i].entityA->set_position(newStateA.p);
		}

		//sphere / sphere
		if (collisions[i].collisionType == collision_data::SPHERE_SPHERE)
		{
			float time_no_collision = (delta_time * collisions[i].time_step);
			float time_after_collision = delta_time - time_no_collision;

			//move back to the point of contact
			glm::vec3 collision_pointA = collisions[i].entityA->get_position() - (collisions[i].entityA->get_velocity() * time_no_collision);
			glm::vec3 collision_pointB = collisions[i].entityB->get_position() - (collisions[i].entityB->get_velocity() * time_no_collision);

			collisions[i].entityA->set_position(collision_pointA);
			collisions[i].entityB->set_position(collision_pointB);

			float massA = collisions[i].entityA->get_mass();
			glm::vec3 velocityA = collisions[i].contactVelocity * (1.0f - velocity_decay);

			float massB = collisions[i].entityB->get_mass();
			glm::vec3 velocityB = collisions[i].contactVelocity_other * (1.0f - velocity_decay);

			glm::vec3 newVelA(0.0f);
			glm::vec3 newVelB(0.0f);

			//handle differend masses (transfer momentum)
			float restitution = 0.42f;
			newVelA = ((((massA - massB) / (massA + massB)) * velocityA) + (((2 * massB) / (massA + massB)) * velocityB) *   restitution);
			newVelB = ((((-massA + massB) / (massA + massB)) * velocityB) + (((2 * massA) / (massA + massB)) * velocityA)  *restitution);
			newVelA += (1.0f - restitution) * collisions[i].normal * glm::dot(newVelB, collisions[i].normal);
			newVelB += (1.0f - restitution) * collisions[i].normal * glm::dot(newVelA, collisions[i].normal);

			collisions[i].entityA->set_velocity(newVelA);
			collisions[i].entityB->set_velocity(newVelB);

			//simulate the rest of the timestep for both spheres
			State newStateA;
			newStateA.p = collisions[i].entityA->get_position();
			newStateA.v = collisions[i].entityA->get_velocity();

			integrate(newStateA, glm::vec3(0.0f), time_after_collision);
			collisions[i].entityA->set_velocity(newStateA.v);
			collisions[i].entityA->set_position(newStateA.p);

			State newStateB;
			newStateB.p = collisions[i].entityB->get_position();
			newStateB.v = collisions[i].entityB->get_velocity();

			integrate(newStateB, glm::vec3(0.0f), time_after_collision);
			collisions[i].entityB->set_velocity(newStateB.v);
			collisions[i].entityB->set_position(newStateB.p);
		}
	}
}
