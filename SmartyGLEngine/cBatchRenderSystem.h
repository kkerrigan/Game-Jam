#pragma once
#include "cInstancedMeshComponent.h"
#include "cEntity.h"

/*!	\class	cBatchRenderSystem
	\brief
	\author Daniel Maclam
	\date	2019-03-20 8:53 PM
	*/
class cBatchRenderSystem
{
public:
	struct instanced_renderer_gpu_data
	{
		instanced_renderer_gpu_data() : cube_vao(0), cube_vbo(0), number_of_vertices(0), model_matrix_vbo(0),
			color_vbo(0),
			number_of_instances(0)
		{
		};
		unsigned int cube_vao;
		unsigned int cube_vbo;
		unsigned int number_of_vertices;
		unsigned int model_matrix_vbo;
		unsigned int color_vbo;
		unsigned int number_of_instances;
		cEntity* entity_prototype;
	};

	instanced_renderer_gpu_data instance_render_data;
	instanced_renderer_gpu_data generate_vbo(cInstancedMeshComponent::batch_info batch_info);
	void instance_draw_test(cEntity* prototype, instanced_renderer_gpu_data instance_render_data);
};

