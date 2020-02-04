#include <cLightManager.h>
#include <cSceneManager.h>

void loadLights() {
	cSceneManager* sceneManager = cSceneManager::getInstance();
	cScene* pScene = sceneManager->getActiveScene();

	cLightManager* lightManager = cLightManager::getInstance();
	lightManager->initializeLights();

	// GRAPHICS CHECKPOINT
	/*sLight* light1 = lightManager->getLightByIndex(0);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 1000.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0001f;
	light1->atten.z = 0.000001f;
	light1->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pScene->addLightToScene(light1);

	sLight* light2 = lightManager->getLightByIndex(1);
	light2->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light2->position = glm::vec4(30.0f, 5.0f, 0.0f, 1.0f);
	light2->atten.z = 0.001f;
	light2->diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pScene->addLightToScene(light2);

	sLight* light3 = lightManager->getLightByIndex(2);
	light3->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light3->position = glm::vec4(-30.0f, 5.0f, 0.0f, 1.0f);
	light3->atten.z = 0.001f;
	light3->diffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	pScene->addLightToScene(light3);

	sLight* light4 = lightManager->getLightByIndex(3);
	light4->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light4->position = glm::vec4(0.0f, 5.0f, 30.0f, 1.0f);
	light4->atten.z = 0.001f;
	light4->diffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	pScene->addLightToScene(light4);

	sLight* light5 = lightManager->getLightByIndex(4);
	light5->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light5->position = glm::vec4(0.0f, 5.0f, -30.0f, 1.0f);
	light5->atten.z = 0.001f;
	light5->atten.y = 0.01f;
	light5->diffuse = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	pScene->addLightToScene(light5);*/

	//// PHYSICS PROJECT 2 SPOTLIGHT
	sLight* light1 = lightManager->getLightByIndex(1);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(5.0f, 20.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light1->SetLightType(sLight::SPOT_LIGHT);
	light1->SetSpotConeAngles(25.0f, 25.0f);
	light1->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	pScene->addLightToScene(light1);
}