#include <cSceneManager.h>
#include <cDebugRenderer.h>
#include <sModelDrawInfo.h>
#include <cVAOMeshManager.h>
#include <cRenderMeshComponent.h>
#include <cTransformComponent.h>

#include <vector>

glm::vec3 converVertsToWorld(glm::vec3 point, glm::vec3 position, glm::quat orientation, glm::vec3 scale) {

	glm::vec4 contactPoint_ModelSpace = glm::vec4(point, 1.0f);

	glm::mat4 matTransform = glm::mat4(1.0f);	// identity matrix

	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), position);
	matTransform = matTransform * matTranslation;		// matMove

	// Now orientation
	glm::quat qRotation = orientation;
	// Make a mat4x4 from that quaternion
	glm::mat4 matQrotation = glm::mat4(qRotation);

	matTransform = matTransform * matQrotation;

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f), scale);

	matTransform = matTransform * matScale;

	// Calcualte the collision points
	glm::vec4 contactPoint_WorldSpace = glm::vec4(0.0f);
	return  matTransform * contactPoint_ModelSpace;
}


void showModelNormals() {

	cDebugRenderer* debug = cDebugRenderer::getInstance();
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();
	
	cEntity* pEntity = pScene->getSelectedEntity();
	cRenderMeshComponent* pCurMesh = static_cast<cRenderMeshComponent*>(pEntity->components[3]);
	cTransformComponent* pCurTrans = static_cast<cTransformComponent*>(pEntity->components[1]);
	
	sModelDrawInfo drawInfo;
	drawInfo.meshFileName = pCurMesh->meshName;

	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(drawInfo);

	for (unsigned int index = 0; index != drawInfo.numberOfVertices; ++index) {

		sPlyVertex curVertex = drawInfo.pVerticesFromFile[index];

		glm::vec3 vertex = glm::vec3(curVertex.x, curVertex.y, curVertex.z);
		glm::vec3 worldVertex = converVertsToWorld(vertex, pCurTrans->get_position(), pCurTrans->getQOrientation(), pCurTrans->scale);
		glm::vec3 normal = glm::normalize(glm::vec3(curVertex.nx, curVertex.ny, curVertex.nz));

		glm::vec3 startPoint = worldVertex;
		glm::vec3 endPoint = startPoint + normal;

		debug->addLine(startPoint, endPoint, glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

