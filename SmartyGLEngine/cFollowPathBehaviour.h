#pragma once
#include "iBehaviour.h"
#include "cPathFinding.h"

/*!	\class	cFollowPathBehaviour
	\brief
	\author Daniel Maclam
	\date	2019-03-03 5:58 PM
	*/
class cFollowPathBehaviour : public iBehaviour
{
	bool is_finished;
	int current_node;
	int direction;

	std::vector<glm::vec3> nodes;
public:
	cFollowPathBehaviour();
	void update(cEntity* entity, float delta_time) override;

	void add_node(glm::vec3 node, int index = -1);
	void set_path(std::vector<glm::vec3> nodes);
	void set_path(cPathFinding::path_info nodes);
	glm::vec3 get_last_node();
	bool is_finsihed() const;
};
