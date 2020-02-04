#pragma once
#include "cComponent.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <map>
#include "cPathFinding.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

/*!	\class	cInstancedMeshComponent
	\brief
	\author Daniel Maclam
	\date	2019-03-16 9:25 PM
	*/
class cInstancedMeshComponent : public cComponent
{
public:
	struct mesh_instance
	{
		int id;
		glm::vec3 position;
		glm::vec4 color;
		glm::quat orientation;
	};

	struct batch_info {
		std::vector<mesh_instance*> instances;
		size_t batch_size;

		batch_info() : batch_size(0) {};
	};

	std::vector<cInstancedMeshComponent::batch_info>  generate_batches();
	void update_map(std::vector<cPathFinding::node*> nodes, bool render_traversable = false);
	std::vector<mesh_instance> instances;
public:
	cInstancedMeshComponent() : cComponent(12) {}
};

