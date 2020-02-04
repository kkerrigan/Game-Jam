#include "cScriptingCommandSystem.h"
#include "cMoveToTime.h"

#include "cSceneManager.h"
#include "cOrientToEntity.h"
#include "cFollowEntity.h"

bool load_cutscene()
{
	//cCommandGroup* masterCommandGroup = new cCommandGroup("master");

	//cEntity* entityToMove = cSceneManager::getInstance()->getActiveScene()->getSelectedEntity();
	//cEntity* terrain = cSceneManager::getInstance()->getActiveScene()->getEntities()[1];

	////rotation point
	//cEntity* rotationPoint = cSceneManager::getInstance()->getActiveScene()->findObjectById(6);
	//cEntity* landingPoint = cSceneManager::getInstance()->getActiveScene()->findObjectById(7);
	//cEntity* stopPoint = cSceneManager::getInstance()->getActiveScene()->findObjectById(5);

	//cEntity* rect1 = cSceneManager::getInstance()->getActiveScene()->findObjectById(4);
	//
	////cEntity* rect3 = cSceneManager::getInstance()->getActiveScene()->getEntities()[4];

	//
	////1
	//cOrientToEntity* orientToPosition = new cOrientToEntity(entityToMove, rotationPoint, 10.0f);
	//cCommandGroup* rotate1CommandGroup = new cCommandGroup("rotate for takeoff");
	//rotate1CommandGroup->commands.push_back(orientToPosition);
	//masterCommandGroup->commandGroups.push_back(rotate1CommandGroup);

	//cMoveToTime* moveToRot = new cMoveToTime(entityToMove, static_cast<cTransformComponent*>(rotationPoint->components[1])->position, 5.0f, 1.5f, 0.0f);
	//cCommandGroup* accelToRot = new cCommandGroup("accellerate towards rotation point");
	//accelToRot->commands.push_back(moveToRot);
	//masterCommandGroup->commandGroups.push_back(accelToRot);

	//cCommandGroup* flyToRectGrp = new cCommandGroup("fly at rect");
	//cMoveToTime* moveAgain2 = new cMoveToTime(entityToMove, (static_cast<cTransformComponent*>(rect1->components[1])->position - glm::vec3(0.0f,10.0f,0.0f)), 30.0f, 0.0f, 3.0f);
	//flyToRectGrp->commands.push_back(moveAgain2);

	//cOrientToEntity* orientToPosition3 = new cOrientToEntity(entityToMove, rect1, 10.0f);
	//flyToRectGrp->commands.push_back(orientToPosition3);
	//masterCommandGroup->commandGroups.push_back(flyToRectGrp);

	//cCommandGroup* flyToLandGrp = new cCommandGroup("fly at landing point");
	//cMoveToTime* moveAgain3 = new cMoveToTime(entityToMove, (static_cast<cTransformComponent*>(landingPoint->components[1])->position), 30.0f, 0.0f, 3.0f);
	//flyToLandGrp->commands.push_back(moveAgain3);

	//cOrientToEntity* orientToPosition4 = new cOrientToEntity(entityToMove, landingPoint, 10.0f);
	//flyToLandGrp->commands.push_back(orientToPosition4);
	//masterCommandGroup->commandGroups.push_back(flyToLandGrp);

	//cCommandGroup* stopPointGrp = new cCommandGroup("stop point");
	//cMoveToTime* moveAgain4 = new cMoveToTime(entityToMove, (static_cast<cTransformComponent*>(stopPoint->components[1])->position), 30.0f, 0.0f, 3.0f);
	//stopPointGrp->commands.push_back(moveAgain4);

	//cOrientToEntity* orientToPosition5 = new cOrientToEntity(entityToMove, stopPoint, 1.0f);
	//stopPointGrp->commands.push_back(orientToPosition5);
	//masterCommandGroup->commandGroups.push_back(stopPointGrp);

	////follow
	//cFollowEntity* followEntity = new cFollowEntity(
	//	cSceneManager::getInstance()->getActiveScene()->findObjectById(8),
	//	cSceneManager::getInstance()->getActiveScene()->getSelectedEntity(),
	//	glm::vec3(1.0f),
	//	10.0f,
	//	50.0f,
	//	500.0f,
	//	glm::vec3(0.0f, 10.0f, 10.0f),
	//	0.0f,
	//	0.0f
	//);

	//masterCommandGroup->commands.push_back(followEntity);

	//cFollowEntity* followEntity2 = new cFollowEntity(
	//	cSceneManager::getInstance()->getActiveScene()->findObjectById(9),
	//	cSceneManager::getInstance()->getActiveScene()->getSelectedEntity(),
	//	glm::vec3(1.0f),
	//	10.0f,
	//	50.0f,
	//	500.0f,
	//	glm::vec3(0.0f, 0.0f, -13.0f),
	//	0.0f,
	//	0.0f
	//);
	//
	//masterCommandGroup->commands.push_back(followEntity2);

	//cScriptingCommandSystem::getInstance()->setMasterCommandGroup(masterCommandGroup);

	return true;
}
