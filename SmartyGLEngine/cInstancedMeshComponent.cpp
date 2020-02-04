/*
* Name:		cInstancedMeshComponent.h
* Author:	Daniel Maclam
* Date		2019-03-16 9:25 PM
*/

#include "cInstancedMeshComponent.h"
#include <map>

#define INSTANCED_MAX_BATCH_SIZE 1000
std::vector<cInstancedMeshComponent::batch_info> cInstancedMeshComponent::generate_batches()
{
	//for each batch 
	size_t batch_index = 0;
	size_t max_batch_size = this->instances.size() < INSTANCED_MAX_BATCH_SIZE ? this->instances.size() : INSTANCED_MAX_BATCH_SIZE;
	size_t number_of_batches = this->instances.size() / max_batch_size + 1;
	size_t processed_instances = 0;
	std::vector<batch_info> batches;
	
	for (size_t i = 0; i < number_of_batches; ++i)
	{
		batch_info batch;
		
		//for each instance in the batch
		size_t end_offset = processed_instances + max_batch_size;
		for(size_t j=processed_instances; j < end_offset; ++j)
		{
			batch.instances.push_back(&this->instances[j]);
			batch.batch_size += 1;
			++processed_instances;

			if(processed_instances==this->instances.size()) break;
		}

		batches.push_back(batch);

		if(processed_instances==this->instances.size()) return batches;
	}

	return batches;
}

void cInstancedMeshComponent::update_map(std::vector<cPathFinding::node*> nodes, bool render_traversable)
{
	instances.clear();

	for (size_t i = 0; i < nodes.size(); ++i)
	{
		mesh_instance new_instance;
		new_instance.position = nodes[i]->position;
		new_instance.position.y -= 0.98f;
		if (!nodes[i]->is_traversable)
		{
			new_instance.position.y += 1.98f;
			new_instance.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			this->instances.push_back(new_instance);
		}

		if (nodes[i]->is_resource)
		{
			new_instance.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			this->instances.push_back(new_instance);
		}

		if (nodes[i]->is_gatherer_spawn)
		{
			new_instance.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			this->instances.push_back(new_instance);
		}

		if (nodes[i]->is_base)
		{
			new_instance.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			this->instances.push_back(new_instance);
		}

		if (render_traversable && nodes[i]->is_traversable)
		{
			new_instance.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			this->instances.push_back(new_instance);
		}
	}
}
