/*
* Name:		cGathererBehaviour.h
* Author:	Daniel Maclam
* Date		2019-03-19 9:46 PM
*/

#include "cGathererBehaviour.h"
#include "cWaitBehaviour.h"
#include "cScene.h"
#include "cFollowPathBehaviour.h"

cGathererBehaviour::cGathererBehaviour(const float base_wait, const float resource_wait, cScene* scene) :
	time_since_last_state_change(0.0f), scene(scene),
	current_state(states::idle_base), base_wait(base_wait),
	resource_wait(resource_wait)
{
	this->active_behaviour = new cWaitBehaviour(this->base_wait);
}

void cGathererBehaviour::update(cEntity* entity, float delta_time)
{
	time_since_last_state_change += delta_time;

	//waiting at base...gonna find a resource soon
	cWaitBehaviour* wait_behaviour = dynamic_cast<cWaitBehaviour*>(this->active_behaviour);
	if (wait_behaviour && this->current_state == states::idle_base)
	{
		wait_behaviour->update(nullptr, delta_time); //entity not needed

		if (wait_behaviour->is_finished())
		{
			//change the state
			this->current_state = states::finding_resource;
			this->set_active_behaviour(nullptr);
			return;
		}
	}

	//find the path
	if (this->current_state == states::finding_resource)
	{
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(entity->components[1]);
		//this is just to roughly find the closest base (we will then use full dijkstra to to find the real path)
		cPathFinding::node* closest_node = this->scene->path_finding->find_path_to_closest_node_of_type_dijkstra(transform_component->get_position(), cLoadBitmapMap::color_types::resource);
		cPathFinding::path_info path_info = this->scene->path_finding->find_path_dijkstra(transform_component->get_position(), this->scene->path_finding->find_closest_node(transform_component->get_position()), closest_node);

		//change the state
		this->current_state = states::traveling_to_resource;

		//no valid paths to resources
		if (!path_info.nodes.empty())
		{
			cFollowPathBehaviour* follow_path_behaviour = new cFollowPathBehaviour();
			follow_path_behaviour->set_path(path_info);
			this->set_active_behaviour(follow_path_behaviour);
			return;
		}

		//no path so return to base
		this->current_state = states::finding_base;
		this->set_active_behaviour(nullptr);
		return;

	}

	//follow the path
	cFollowPathBehaviour* follow_path_behaviour = dynamic_cast<cFollowPathBehaviour*>(this->active_behaviour);
	if (follow_path_behaviour && this->current_state == states::traveling_to_resource)
	{
		//if the node is in use (someone got there before us and gets the resource)
		if (this->scene->path_finding->find_closest_node(follow_path_behaviour->get_last_node())->is_in_use)
		{
			this->current_state = states::finding_resource;
			this->set_active_behaviour(nullptr);
			return;
		}

		cTransformComponent* transform_component = static_cast<cTransformComponent*>(entity->components[1]);

		follow_path_behaviour->update(entity, delta_time);
		if (follow_path_behaviour->is_finsihed())
		{
			this->current_state = states::idle_resource;
			this->set_active_behaviour(new cWaitBehaviour(this->resource_wait));

			//mark the node as in use
			this->scene->path_finding->find_closest_node(transform_component->get_position())->is_in_use = true;
		}
	}

	//wait and gather the resource
	cWaitBehaviour* wait_resource_behaviour = dynamic_cast<cWaitBehaviour*>(this->active_behaviour);
	if (wait_resource_behaviour && this->current_state == states::idle_resource)
	{
		wait_resource_behaviour->update(nullptr, delta_time); //entity not needed

		if (wait_resource_behaviour->is_finished())
		{
			//change the state
			this->current_state = states::finding_base;
			this->set_active_behaviour(nullptr);

			cTransformComponent* transform_component = static_cast<cTransformComponent*>(entity->components[1]);
			this->scene->path_finding->find_closest_node(transform_component->get_position())->is_resource = false;
			return;
		}
	}

	//find the path to base
	if (this->current_state == states::finding_base)
	{
		this->set_active_behaviour(nullptr);
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(entity->components[1]);
		//this is just to roughly find the closest base (we will then use a* to find the real path)
		cPathFinding::node* closest_node = this->scene->path_finding->find_path_to_closest_node_of_type_dijkstra(transform_component->get_position(), cLoadBitmapMap::color_types::base);
		cPathFinding::path_info path_info = this->scene->path_finding->find_path_a_star(transform_component->get_position(), this->scene->path_finding->find_closest_node(transform_component->get_position()), closest_node);

		//change the state
		this->current_state = states::traveling_to_base;

		cFollowPathBehaviour* follow_path_base_behaviour = new cFollowPathBehaviour();
		follow_path_base_behaviour->set_path(path_info);
		this->set_active_behaviour(follow_path_base_behaviour);
		return;
	}

	//follow the path to base
	cFollowPathBehaviour* follow_path_base_behaviour = dynamic_cast<cFollowPathBehaviour*>(this->active_behaviour);
	if (follow_path_base_behaviour && this->current_state == states::traveling_to_base)
	{

		follow_path_base_behaviour->update(entity, delta_time);

		if (follow_path_base_behaviour->is_finsihed())
		{
			this->current_state = states::idle_base;
			this->set_active_behaviour(new cWaitBehaviour(this->base_wait));
		}
	}

}

void cGathererBehaviour::set_active_behaviour(iBehaviour* behaviour)
{
	delete this->active_behaviour;
	this->active_behaviour = behaviour;
}
