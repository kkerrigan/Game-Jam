/*
* Name:		cPathFinding.h
* Author:	Daniel Maclam
* Date		2019-03-18 6:01 PM
*/

#include "cPathFinding.h"
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <algorithm>
#include <glm/gtx/norm.inl>

cPathFinding::node::node() : position(glm::vec3(0.0f)), is_traversable(false), is_base(false), is_gatherer_spawn(false), is_resource(false), is_in_use(false), neighbours{ nullptr }
{
	this->neighbours[self] = this;
}

cPathFinding::node* cPathFinding::node::get_neighbour(neighbour_index index)
{
	return this->neighbours[index];
}

void cPathFinding::node::set_neighbour(neighbour_index index, node* neighbour)
{
	this->neighbours[index] = neighbour;
}

//use dijkstra algorithm to find the shortest path
cPathFinding::path_info cPathFinding::find_path_dijkstra(glm::vec3 current_position, node* start_node, node* goal_node)
{
	std::vector<node*> open_set;
	std::vector<node*> closed_set;

	//set all the nodes distance to FLT_MAX
	for (int i = 0; i < this->nodes.size(); ++i)
	{
		this->nodes[i]->parent = nullptr;
		this->nodes[i]->current_node_distance = FLT_MAX;
	}

	//set the distance to the start node to 0.0f
	start_node->current_node_distance = 0.0f;

	//add the start node to the open set
	this->add_node_to_open_set(open_set, start_node);
	return this->calculate_path_dijkstra(open_set, closed_set, goal_node);
}

cPathFinding::path_info cPathFinding::find_path_a_star(glm::vec3 current_position, node* start_node, node* goal_node)
{
	std::vector<node*> open_set;
	std::vector<node*> closed_set;

	//set all the nodes distance to FLT_MAX
	for (int i = 0; i < this->nodes.size(); ++i)
	{
		this->nodes[i]->parent = nullptr;
		this->nodes[i]->current_node_distance = FLT_MAX;
	}

	//set the distance to the start node to 0.0f
	start_node->current_node_distance = 0.0f;

	//add the start node to the open set
	this->add_node_to_open_set(open_set, start_node);
	return this->calculate_path_a_star(open_set, closed_set, goal_node);
}
#include <glm/geometric.hpp>
cPathFinding::node* cPathFinding::find_closest_node(glm::vec3 position)
{
	float max_distance = FLT_MAX;
	unsigned long long max_index = 0;

	for (unsigned long long i = 0; i < this->nodes.size(); ++i)
	{
		float dist = glm::abs(glm::distance2(this->nodes[i]->position, position));
		if (dist < max_distance)
		{
			max_distance = dist;
			max_index = i;
		}
	}

	return this->nodes[max_index];
}

std::vector<cPathFinding::node*> cPathFinding::find_spawn_nodes()
{
	std::vector<cPathFinding::node*> result_nodes;
	for (unsigned long long i = 0; i < this->nodes.size(); ++i)
	{
		if (this->nodes[i]->is_gatherer_spawn)
		{
			result_nodes.push_back(this->nodes[i]);
		}
	}

	return result_nodes;
}

std::vector<cPathFinding::node*> cPathFinding::find_resource_nodes()
{
	std::vector<cPathFinding::node*> result_nodes;
	for (unsigned long long i = 0; i < this->nodes.size(); ++i)
	{
		if (this->nodes[i]->is_resource)
		{
			result_nodes.push_back(this->nodes[i]);
		}
	}

	return result_nodes;
}

std::vector<cPathFinding::node*> cPathFinding::find_base_nodes()
{
	std::vector<cPathFinding::node*> result_nodes;
	for (unsigned long long i = 0; i < this->nodes.size(); ++i)
	{
		if (this->nodes[i]->is_base)
		{
			result_nodes.push_back(this->nodes[i]);
		}
	}

	return result_nodes;
}

void cPathFinding::add_node_to_open_set(std::vector<node*>& open_set, node* node)
{
	open_set.push_back(node);
}

void cPathFinding::remove_node_from_open_set(std::vector<node*>& open_set, node* node)
{
	for (auto iter = open_set.begin(); iter != open_set.end(); ++iter)
	{
		if (*iter == node)
		{
			open_set.erase(iter);
			break;
		}
	}
}

void cPathFinding::add_node_to_closed_set(std::vector<node*>& closed_set, node* node)
{
	closed_set.push_back(node);
}

void cPathFinding::remove_node_from_closed_set(std::vector<node*>& closed_set, node* node)
{
	for (auto iter = closed_set.begin(); iter != closed_set.end(); ++iter)
	{
		if (*iter == node)
		{
			closed_set.erase(iter);
			break;
		}
	}
}

int count_d = 0;
cPathFinding::path_info cPathFinding::calculate_path_dijkstra(std::vector<node*>& open_set, std::vector<node*>& closed_set, node* goal_node)
{
	for (;;)
	{
		++count_d;

		//no more nodes to check
		if (open_set.empty()) break;

		//remove lowest cost node
		int max_index = 0;
		float max_dist = FLT_MAX;
		for (size_t i = 0; i < open_set.size(); ++i)
		{
			if (open_set[i]->current_node_distance < max_dist)
			{
				max_dist = open_set[i]->current_node_distance;
				max_index = i;
			}
		}
		node* closest_node = open_set[max_index];
		this->remove_node_from_open_set(open_set, closest_node);

		//add the node to the closed set
		this->add_node_to_closed_set(closed_set, closest_node);

		//are we at the goal?
		if (closest_node == goal_node)
		{
			open_set.clear();
			break;
		}

		//add neighbours to the open set
		for (size_t c = 0; c < 9; c++)
		{
			if (closest_node->neighbours[c] && closest_node->neighbours[c]->is_traversable)
			{
				if (is_node_in_set(open_set, closest_node->neighbours[c]))
				{
					// Check if this is a faster route.. 
					float new_distance = closest_node->current_node_distance + glm::distance(closest_node->neighbours[c]->position, closest_node->position);

					if (closest_node->neighbours[c]->current_node_distance > new_distance)
					{
						closest_node->neighbours[c]->current_node_distance = new_distance;
						closest_node->neighbours[c]->parent = closest_node;
					}

					continue;
				}

				if (is_node_in_set(closed_set, closest_node->neighbours[c])) continue;		// Don't worry about closed nodes..

				//if its a diagonal that is blocked by a wall don't go that way	
				if (c == node::top_left || c == node::top_right || c == node::bottom_left || c == node::bottom_right)
				{
					if (!valid_diagonal_path(closest_node, c)) continue;
				}

				//add it to the open set
				this->add_node_to_open_set(open_set, closest_node->neighbours[c]);

				//set the parent
				closest_node->neighbours[c]->parent = closest_node;

				//and calculate the weight
				closest_node->neighbours[c]->current_node_distance = closest_node->current_node_distance + glm::distance(closest_node->neighbours[c]->position, closest_node->position);
			}
		}
	}

	printf("Dijkstra checked %d nodes.\n", count_d);
	count_d = 0;

	std::vector<glm::vec3> path;
	node* current_node = goal_node;
	while (current_node != nullptr)
	{
		path.push_back(current_node->position);
		current_node = current_node->parent;
	}

	std::reverse(path.begin(), path.end());

	path_info info;
	info.nodes = path;

	return info;

}

int count = 0;
cPathFinding::path_info cPathFinding::calculate_path_a_star(std::vector<node*>& open_set,
	std::vector<node*>& closed_set, node* goal_node)
{
	for (;;)
	{
		++count;
		//no more nodes to check
		if (open_set.empty()) break;

		//remove lowest cost node (favoring nodes that are closer to the goal node..euclidean distance that is)
		int max_index = 0;
		float max_dist = FLT_MAX;
		for (size_t i = 0; i < open_set.size(); ++i)
		{
			if (open_set[i]->current_node_weight < max_dist)
			{
				max_dist = open_set[i]->current_node_weight;
				max_index = i;
			}
		}
		node* closest_node = open_set[max_index];
		this->remove_node_from_open_set(open_set, closest_node);

		//add the node to the closed set
		this->add_node_to_closed_set(closed_set, closest_node);

		//are we at the goal?
		if (closest_node == goal_node)
		{
			open_set.clear();
			break;
		}

		//add neighbours to the open set
		for (size_t c = 0; c < 9; c++)
		{
			if (closest_node->neighbours[c] && closest_node->neighbours[c]->is_traversable)
			{
				if (is_node_in_set(open_set, closest_node->neighbours[c]))
				{
					closest_node->neighbours[c]->node_distance_to_goal = this->calculate_heuristic(closest_node->neighbours[c], goal_node);

					// Check if this is a faster route
					float new_distance = closest_node->current_node_distance + glm::distance(closest_node->neighbours[c]->position, closest_node->position);
					if (closest_node->neighbours[c]->current_node_distance > new_distance)
					{
						closest_node->neighbours[c]->current_node_distance = new_distance;
						closest_node->neighbours[c]->parent = closest_node;
					}

					continue;
				}

				if (is_node_in_set(closed_set, closest_node->neighbours[c])) continue;		// Don't worry about closed nodes..

				//if its a diagonal that is blocked by a wall don't go that way	
				if (c == node::top_left || c == node::top_right || c == node::bottom_left || c == node::bottom_right)
				{
					if (!valid_diagonal_path(closest_node, c)) continue;
				}

				//add it to the open set
				this->add_node_to_open_set(open_set, closest_node->neighbours[c]);

				//set the parent
				closest_node->neighbours[c]->parent = closest_node;

				//and calculate the weight
				closest_node->neighbours[c]->node_distance_to_goal = this->calculate_heuristic(closest_node->neighbours[c], goal_node);
				closest_node->neighbours[c]->current_node_distance = closest_node->current_node_distance + glm::abs(glm::distance(closest_node->neighbours[c]->position, closest_node->position));
				closest_node->neighbours[c]->current_node_weight = closest_node->neighbours[c]->current_node_distance + glm::pow(closest_node->neighbours[c]->node_distance_to_goal, 2);
			}
		}
	}

	printf("A* checked %d nodes.\n", count);
	count =0;

	std::vector<glm::vec3> path;
	node* current_node = goal_node;
	while (current_node != nullptr)
	{
		path.push_back(current_node->position);
		current_node = current_node->parent;
	}

	std::reverse(path.begin(), path.end());

	path_info info;
	info.nodes = path;

	return info;
}

bool cPathFinding::is_node_in_set(std::vector<node*>& set, node* node)
{
	for (auto iter = set.begin(); iter != set.end(); ++iter)
	{
		if (*iter == node)
		{
			return true;
		}
	}

	return false;
}


bool cPathFinding::valid_diagonal_path(node* closest_node, int diagonal_index)
{
	if (diagonal_index == node::top_left)
	{
		return closest_node->neighbours[node::left]->is_traversable && closest_node->neighbours[node::top]->is_traversable;
	}

	if (diagonal_index == node::top_right)
	{
		return closest_node->neighbours[node::right]->is_traversable && closest_node->neighbours[node::top]->is_traversable;
	}

	if (diagonal_index == node::bottom_left)
	{
		return closest_node->neighbours[node::left]->is_traversable && closest_node->neighbours[node::bottom]->is_traversable;
	}

	if (diagonal_index == node::bottom_right)
	{
		return closest_node->neighbours[node::right]->is_traversable && closest_node->neighbours[node::bottom]->is_traversable;
	}

	return false; //why did we get here?
}

float cPathFinding::calculate_heuristic(node* test_node, node* goal)
{
		float dx = abs(test_node->position.x - goal->position.x);
		float dy = abs(test_node->position.z - goal->position.z);

		return sqrt(dx * dx + dy * dy);
}

cPathFinding::node* cPathFinding::find_path_to_closest_node_of_type_dijkstra(glm::vec3 start_position,
	cLoadBitmapMap::color_types node_type)
{
	std::vector<node*> open_set;
	std::vector<node*> closed_set;

	//short circuit if there is only one of the type of node
	if(node_type == cLoadBitmapMap::resource)
	{
		if(this->find_resource_nodes().size() == 1)
		{
			return this->find_resource_nodes()[0];
		}
	}

	if(node_type == cLoadBitmapMap::base)
	{
		if(this->find_base_nodes().size() == 1)
		{
			return this->find_base_nodes()[0];
		}
	}

	for (int i = 0; i < this->nodes.size(); ++i)
	{
		this->nodes[i]->parent = nullptr;
		this->nodes[i]->current_node_distance = FLT_MAX;
	}

	node* start_node = this->find_closest_node(start_position);
	start_node->current_node_distance = 0;
	open_set.push_back(start_node);

	node* goal_node = nullptr;

	for (;;)
	{
		if (open_set.empty()) //no path
		{
			break;
		}

		//remove lowest cost node
		int max_index = 0;
		float max_dist = FLT_MAX;
		for (size_t i = 0; i < open_set.size(); ++i)
		{

			if (open_set[i]->current_node_distance < max_dist)
			{
				max_dist = open_set[i]->current_node_distance;
				max_index = i;
			}
		}

		node* closest_node = open_set[max_index];

		this->remove_node_from_open_set(open_set, closest_node);

		//add the node to the closed set
		this->add_node_to_closed_set(closed_set, closest_node);

		//are we at the goal? (see wikipedia...If we are only interested in a shortest path between vertices source and target, we can terminate the search after line 15 if u = target. Now we can read the shortest path from source to target by reverse iteration)
		switch (node_type)
		{
		case cLoadBitmapMap::color_types::resource:
			if (closest_node->is_resource && !closest_node->is_in_use)
			{
				goal_node = closest_node;
				open_set.clear();
			}
			break;

		case cLoadBitmapMap::color_types::base:
			if (closest_node->is_base)
			{
				goal_node = closest_node;
				open_set.clear();
			}
			break;
		}

		//found a goal?
		if (goal_node)
		{
			open_set.clear();

			return goal_node;
		}

		//add neighbours to the open set
		for (size_t c = 0; c < 9; c++)
		{
			if (closest_node->neighbours[c] && closest_node->neighbours[c]->is_traversable)
			{
				if (is_node_in_set(open_set, closest_node->neighbours[c]))
				{
					// Check if this is a faster route.. 
					float new_distance = closest_node->current_node_distance + glm::distance(closest_node->neighbours[c]->position, closest_node->position);

					if (closest_node->neighbours[c]->current_node_distance > new_distance)
					{
						closest_node->neighbours[c]->current_node_distance = new_distance;
						closest_node->neighbours[c]->parent = closest_node;
					}

					continue;
				}

				if (is_node_in_set(closed_set, closest_node->neighbours[c])) continue;		// Don't worry about closed nodes..

				//if its a diagonal that is blocked by a wall don't go that way	
				if (c == node::top_left || c == node::top_right || c == node::bottom_left || c == node::bottom_right)
				{
					if (!valid_diagonal_path(closest_node, c)) continue;
				}

				//add it to the open set
				this->add_node_to_open_set(open_set, closest_node->neighbours[c]);

				//set the parent
				closest_node->neighbours[c]->parent = closest_node;

				//and calculate the weight
				closest_node->neighbours[c]->current_node_distance = closest_node->current_node_distance + glm::distance(closest_node->neighbours[c]->position, closest_node->position);
			}
		}
	}

	return nullptr;
}
