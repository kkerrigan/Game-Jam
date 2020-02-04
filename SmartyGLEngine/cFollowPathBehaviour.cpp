/*
* Name:		cFollowPathBehaviour.h
* Author:	Daniel Maclam
* Date		2019-03-03 5:58 PM
*/

#include "cFollowPathBehaviour.h"
#include "cPathFinding.h"

cFollowPathBehaviour::cFollowPathBehaviour(): is_finished(false), current_node(0), direction(1)
{
}

void cFollowPathBehaviour::update(cEntity* entity, float delta_time)
{
	if(is_finished) return;

	cTransformComponent* transform_component = static_cast<cTransformComponent*>(entity->components[1]);

	//check if we are at a node
	glm::vec3 entity_position = transform_component->get_position();
	glm::vec3 current_node_position = this->nodes[this->current_node];
	float distance_to_current_node = glm::abs(glm::distance(entity_position, current_node_position));

	//if we are at the node then change the next node if we have one
	if(distance_to_current_node <= 0.05f)
	{
		//goal node (reverse direction)
		size_t end_node = this->nodes.size() - 1;
		
		//we are at the end node so stop
		if(this->nodes[this->current_node] == this->nodes[end_node])
		{
			is_finished = true;
			return;
		}

		this->current_node = (this->current_node + direction);
	}

	current_node_position = this->nodes[this->current_node];

	//change the entities directions to face the current node
	glm::vec3 direction = glm::normalize(current_node_position - entity_position);
	glm::quat orientation_to_current_node = glm::quat(glm::inverse(glm::lookAt(entity_position, entity_position - direction, glm::vec3(0.0f, 1.0f, 0.0f))));

	//if we are close to the current node start turning towards the next node
	distance_to_current_node = glm::distance(entity_position, current_node_position);
	
	if(distance_to_current_node >= 3.0f)
	{
		orientation_to_current_node = glm::mix(transform_component->getQOrientation(), orientation_to_current_node, 0.05f);	
	} 
	
	transform_component->setQOrientation(orientation_to_current_node);

	glm::vec3 forward = glm::toMat3(transform_component->getQOrientation()) * glm::vec3(0.0f, 0.0f, 1.0f);

	transform_component->set_position(entity_position + forward * delta_time);
}

void cFollowPathBehaviour::add_node(glm::vec3 node, int index)
{
	//no index passed
	if(index == -1)
	{
		this->nodes.push_back(node);
		return;
	}

	std::vector<glm::vec3>::iterator it = this->nodes.begin() + index;
	this->nodes.insert(it, 1, node);
}

void cFollowPathBehaviour::set_path(std::vector<glm::vec3> nodes)
{
	this->nodes = nodes;
}

void cFollowPathBehaviour::set_path(cPathFinding::path_info path_info)
{
	this->nodes = path_info.nodes;
}

glm::vec3 cFollowPathBehaviour::get_last_node()
{
	return this->nodes.back();
}

bool cFollowPathBehaviour::is_finsihed() const
{
	return this->is_finished;
}
