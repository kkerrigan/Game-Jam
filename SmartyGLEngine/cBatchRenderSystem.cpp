/*
* Name:		cBatchRenderSystem.h
* Author:	Daniel Maclam
* Date		2019-03-20 8:53 PM
*/
#include "cEngine.h"
#include "cBatchRenderSystem.h"
#include "cShaderManager.h"
#include "sVertex_xyz_rgba_n_uv2_bt_4Bones.h"
#include "sModelDrawInfo.h"
#include "cVAOMeshManager.h"
#include "cMeshRenderSystem.h"

cBatchRenderSystem::instanced_renderer_gpu_data cBatchRenderSystem::generate_vbo(cInstancedMeshComponent::batch_info batch_info)
{
	unsigned int amount = batch_info.instances.size();
	instance_render_data.number_of_instances = amount;
	glm::mat4* modelMatrices = new glm::mat4[amount];

	unsigned int real_num_of_models = 0;
	for (unsigned int i = 0; i < amount; i++)
	{
		//position
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), batch_info.instances[i]->position);
		model = model * matTranslation;

		glm::mat4 matQrotation = glm::mat4(batch_info.instances[i]->orientation);
		model *= matQrotation;

		//scale
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
		model = model * scale;
		modelMatrices[real_num_of_models] = model;

		real_num_of_models++;
	}

	if (instance_render_data.model_matrix_vbo == 0)
		glGenBuffers(1, &this->instance_render_data.model_matrix_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->instance_render_data.model_matrix_vbo);
	glBufferData(GL_ARRAY_BUFFER, 64 * real_num_of_models, &modelMatrices[0], GL_DYNAMIC_DRAW);


	delete[] modelMatrices;

	sModelDrawInfo model_draw_info;
	model_draw_info.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";
	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(model_draw_info);

	if (model_draw_info.current_state == sModelDrawInfo::state::ready_for_gpu)
	{
		cVAOMeshManager::getInstance()->LoadModelIntoVAO(model_draw_info, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"));
	}

	if (model_draw_info.current_state != sModelDrawInfo::state::loaded)
	{
		return cBatchRenderSystem::instanced_renderer_gpu_data();
	}

	this->instance_render_data.number_of_vertices = model_draw_info.numberOfVertices;

	// also set instance data
	int attrib_loc = glGetAttribLocation(cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), "instanceMatrix");

	int pos = attrib_loc;
	int pos1 = pos + 0;
	int pos2 = pos + 1;
	int pos3 = pos + 2;
	int pos4 = pos + 3;

	//the model matrices (instanced)
	glBindBuffer(GL_ARRAY_BUFFER, this->instance_render_data.model_matrix_vbo);

	//enable the attributes
	glEnableVertexAttribArray(pos1);
	glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(0));

	glEnableVertexAttribArray(pos2);
	glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(sizeof(float) * 4));

	glEnableVertexAttribArray(pos3);
	glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(sizeof(float) * 8));

	glEnableVertexAttribArray(pos4);
	glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(sizeof(float) * 12));

	//per instance
	glVertexAttribDivisor(pos1, 1);
	glVertexAttribDivisor(pos2, 1);
	glVertexAttribDivisor(pos3, 1);
	glVertexAttribDivisor(pos4, 1);

	return instance_render_data;
}

void cBatchRenderSystem::instance_draw_test(cEntity* prototype, instanced_renderer_gpu_data instance_render_data)
{
	glUniform1f(glGetUniformLocation(cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), "is_instanced"), GL_TRUE);

	sModelDrawInfo model_draw_info;
	model_draw_info.meshFileName = static_cast<cRenderMeshComponent*>(prototype->components[3])->meshName;
	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(model_draw_info);

	if (model_draw_info.current_state == sModelDrawInfo::state::ready_for_gpu)
	{
		cVAOMeshManager::getInstance()->LoadModelIntoVAO(model_draw_info, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"));
	}

	if (model_draw_info.current_state != sModelDrawInfo::state::loaded)
	{
		return;
	}

	glUniform1f(glGetUniformLocation(cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), "bDontUseLighting"), (float)GL_FALSE);

	cShaderManager::getInstance()->useShaderProgram("BasicUberShader");
	glBindVertexArray(model_draw_info.VAO_ID);

	glBindBuffer(GL_ARRAY_BUFFER, instance_render_data.model_matrix_vbo);

	int attrib_loc = glGetAttribLocation(cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), "instanceMatrix");

	int pos = attrib_loc;
	int pos1 = pos + 0;
	int pos2 = pos + 1;
	int pos3 = pos + 2;
	int pos4 = pos + 3;

	//enable the attributes
	glEnableVertexAttribArray(pos1);
	glVertexAttribPointer(pos1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(0));

	glEnableVertexAttribArray(pos2);
	glVertexAttribPointer(pos2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(sizeof(float) * 4));

	glEnableVertexAttribArray(pos3);
	glVertexAttribPointer(pos3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(sizeof(float) * 8));

	glEnableVertexAttribArray(pos4);
	glVertexAttribPointer(pos4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4 * 4, (void*)(sizeof(float) * 12));

	//per instance
	glVertexAttribDivisor(pos1, 1);
	glVertexAttribDivisor(pos2, 1);
	glVertexAttribDivisor(pos3, 1);
	glVertexAttribDivisor(pos4, 1);

	glDrawElementsInstanced(GL_TRIANGLES,
		model_draw_info.numberOfIndices,
		GL_UNSIGNED_INT,
		nullptr,
		this->instance_render_data.number_of_instances);

	glBindVertexArray(0);

	glUniform1f(glGetUniformLocation(cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), "is_instanced"), GL_FALSE);
}
