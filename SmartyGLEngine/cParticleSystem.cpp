#include "cParticleSystem.h"
#include "cParticleComponent.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"
#include "cSceneManager.h"
#include "cShaderManager.h"
#include "cMeshRenderSystem.h"

cParticleSystem::cParticleSystem() {}

cParticleSystem::~cParticleSystem(){}

cParticleSystem* cParticleSystem::getInstance() {
	
	static cParticleSystem instance;

	return &instance;
}

void cParticleSystem::update(double deltaTime) {

	cScene* pScene = cSceneManager::getInstance()->getActiveScene();
	glm::vec3 cameraEye = pScene->flyCamera.eye;
	 
	cShaderManager::getInstance()->useShaderProgram("BasicUberShader"); // Don't know why this fixed it, but it do.
	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	GLint bIsParticleImposter_UniLoc = glGetUniformLocation(program, "bIsParticleImposter");
	GLint ParticleImposterAlphaOverride_UniLoc = glGetUniformLocation(program, "ParticleImposterAlphaOverride");
	GLint ParticleImposterBlackThreshold_UniLoc = glGetUniformLocation(program, "ParticleImposterBlackThreshold");

	// Black threshold is where the imposter will discard 
	glUniform1f(ParticleImposterBlackThreshold_UniLoc, 0.25f);

	for (unsigned int index = 0; index != this->entitiesToUpdate.size(); ++index) {

		cParticleComponent* pCurParticle = static_cast<cParticleComponent*>(this->entitiesToUpdate[index]->components[7]);
		cTransformComponent* pEntityTransform = static_cast<cTransformComponent*>(this->entitiesToUpdate[index]->components[1]);
		cEntity* pParticle = pCurParticle->particleEntity;
		cTransformComponent* pParticleTrans = static_cast<cTransformComponent*>(pParticle->components[1]);
		glm::vec3 oldPosition = pParticleTrans->get_position();
		glm::quat oldOrientation = pParticleTrans->getQOrientation();
		glm::vec3 oldScale = pParticleTrans->scale;
		pParticleTrans->setOrientationEulerAngles(0.0f, 0.0f, 0.0f);

		cRenderMeshComponent* pParticleMesh = static_cast<cRenderMeshComponent*>(pParticle->components[3]);
		pParticleMesh->bIsVisible = true;

		glm::mat4 matIdentity = glm::mat4(1.0f);

		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_TRUE);

		pCurParticle->update(deltaTime);

		// Get the particles to draw
		std::vector<sParticle> vecParticlesToDraw;
		pCurParticle->getAliveParticles(vecParticlesToDraw);
		pCurParticle->sortParticlesBackToFront(vecParticlesToDraw, cameraEye);

		unsigned int numParticles = (unsigned int)vecParticlesToDraw.size();
		unsigned int count = 0;
		for (unsigned int index = 0; index != numParticles; ++index) {

			if (vecParticlesToDraw[index].lifeRemaining > 0.0f) {
				// Draw it
				pParticleTrans->set_position(vecParticlesToDraw[index].position + pEntityTransform->get_position());
				pParticleTrans->scale = glm::vec3(vecParticlesToDraw[index].scale);
				pParticleTrans->setQOrientation(vecParticlesToDraw[index].qOrientation);

				// This is for the "death" transparency
				glUniform1f(ParticleImposterAlphaOverride_UniLoc, vecParticlesToDraw[index].transparency);

				cMeshRenderSystem::getInstance()->drawObject(pParticle, matIdentity, program, 1, 0.0f);
				count++;
			}
		}

		//std::cout << "Drew " << count << " particles" << std::endl;
		pParticleMesh->bIsVisible = false;
		pParticleTrans->set_position(oldPosition);
		pParticleTrans->setQOrientation(oldOrientation);
		pParticleTrans->scale = oldScale;
		glUniform1f(bIsParticleImposter_UniLoc, (float)GL_FALSE);
		glUniform1f(ParticleImposterAlphaOverride_UniLoc, 1.0f);
	}

	return;
}

void cParticleSystem::registerEntity(cEntity* entity) {

	this->entitiesToUpdate.push_back(entity);

	return;
}

void cParticleSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector

	return;
}

