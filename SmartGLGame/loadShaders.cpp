#include <Global.h>
#include <cShaderManager.h>
#include <iostream>

bool loadShaders() {
	// load shaders
	cShaderManager* pShaderManager = cShaderManager::getInstance();
	pShaderManager->setBasePath("./assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (!pShaderManager->createProgramFromFile("BasicUberShader", vertexShader, fragmentShader)) {
		std::cout << "Compiled shaders OK." << std::endl;
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;

		return false;
	}

	cShaderManager::cShader vertexShaderFT;
	cShaderManager::cShader fragmentShaderFT;

	vertexShaderFT.fileName = "freetype_v.glsl";
	vertexShaderFT.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShaderFT.fileName = "freetype_f.glsl";
	fragmentShaderFT.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (!pShaderManager->createProgramFromFile("FreeTypeShader", vertexShaderFT, fragmentShaderFT)) {
		
		return false;
	}

	cShaderManager::cShader instanced_v;
	cShaderManager::cShader instanced_f;

	instanced_v.fileName = "instanced_v.glsl";
	instanced_v.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	instanced_f.fileName = "instanced_f.glsl";
	instanced_f.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (!pShaderManager->createProgramFromFile("InstancedShader", instanced_v, instanced_f)) {
		std::cout << "Compiled shaders OK." << std::endl;
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;

		return false;
	}




	return true;
}