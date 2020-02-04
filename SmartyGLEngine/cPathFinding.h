#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "cLoadBitmapMap.h"

/*!	\class	cPathFinding
	\brief
	\author Daniel Maclam
	\date	2019-03-18 6:01 PM
	*/
class cPathFinding
{
public:
	struct path_info
		{
			std::vector<glm::vec3> nodes;
			float path_length;
		};

	struct node
	{
		glm::vec3 position;
		bool is_traversable;
		bool is_base;
		bool is_gatherer_spawn;
		bool is_resource;
		bool is_in_use;

		//these are used to keep track of a path
		node* parent = nullptr;
		float current_node_distance;
		float node_distance_to_goal;
		float current_node_weight; //this is a combination of the current_node_distance && node_distance_to_goal

		/*
		 * Neighbours map
		 * 
		 *	[0][1][2]
		 *	[3][4][5]		//4 is always this node
		 *	[6][7][8]
		 */
		enum neighbour_index
		{
			top_left = 0,
			top = 1,
			top_right = 2,
			left = 3,
			self = 4,
			right = 5,
			bottom_left = 6,
			bottom = 7,
			bottom_right = 8
		};
		
		node* neighbours[9];

		node();

		node* get_neighbour(neighbour_index index);
		void set_neighbour(neighbour_index index, node* neighbour);
	};

	std::vector<node*> nodes;
	path_info find_path_dijkstra(glm::vec3 current_position, node* start_node, node* goal_node);
	path_info find_path_a_star(glm::vec3 current_position, node* start_node, node* goal_node);

	node* find_closest_node(glm::vec3 position);
	std::vector<node*> find_spawn_nodes();
	std::vector<node*> find_resource_nodes();
	std::vector<node*> find_base_nodes();

	node* find_path_to_closest_node_of_type_dijkstra(glm::vec3 start_position, cLoadBitmapMap::color_types node_type);

private:
	void add_node_to_open_set(std::vector<node*>& open_set, node* node);
	void remove_node_from_open_set(std::vector<node*>& open_set, node* node);
	void add_node_to_closed_set(std::vector<node*>& closed_set, node* node);
	void remove_node_from_closed_set(std::vector<node*>& closed_set, node* node);

	path_info calculate_path_dijkstra(std::vector<node*>& open_set, std::vector<node*>& closed_set, node* goal_node);
	path_info calculate_path_a_star(std::vector<node*>& open_set, std::vector<node*>& closed_set, node* goal_node);
	bool is_node_in_set(std::vector<node*>& set, node* node);
	bool valid_diagonal_path(node* closest_node, int diagonal_index);
	float calculate_heuristic(node* test_node, node* goal);

	
};

