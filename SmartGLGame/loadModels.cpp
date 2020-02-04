#include <cVAOMeshManager.h>
#include <cShaderManager.h>
#include <iostream>
#include "cCameraMoveToTime.h"
#include "app_optomizations.h"

double start_time = 0.0f;
double end_time = 0.0f;

void loadModels() {

	start_time = glfwGetTime();
	cVAOMeshManager* pMeshManager = cVAOMeshManager::getInstance();
	pMeshManager->setBasePath("./assets/models/");
	int shaderProgramId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	//MUST HAVE MODELS BEFORE WE CAN RENDER (loaded synchronously)
	//FBO for second pass (can't render anything without this quad)
	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
		{
			std::cout << "Didn't load rect1_xyz_n_uv.ply" << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	if (LOAD_MODELS_THREADED)
	{
		//MODELS THAT CAN LOAD IN ON THERE OWN THREADS
		//skybox
		pMeshManager->m_LoadModelFromFile_Threaded("Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply");

		//terrain
		{
			sModelDrawInfo draw_info;
			draw_info.meshFileName ="island.ply";
			pMeshManager->LoadModelIntoVAO(draw_info, shaderProgramId);	
		}
		
		{
			sModelDrawInfo draw_info;
			draw_info.meshFileName ="b1.ply";
			pMeshManager->LoadModelIntoVAO(draw_info, shaderProgramId);	
		}
		

		//flat
		pMeshManager->m_LoadModelFromFile_Threaded("flat.ply");

		//fire pit
		pMeshManager->m_LoadModelFromFile_Threaded("firepit.ply");

		//smoke imposter
		pMeshManager->m_LoadModelFromFile_Threaded("1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply");

		//cube
		pMeshManager->m_LoadModelFromFile_Threaded("cube_flat_shaded_xyz_n_uv.ply");

		//sphere
		pMeshManager->m_LoadModelFromFile_Threaded("Sphere_320_faces_xyz_n_GARBAGE_uv.ply");

		//capsule
		pMeshManager->m_LoadModelFromFile_Threaded("capsule.ply");

		//cone
		pMeshManager->m_LoadModelFromFile_Threaded("cone.ply");

		//cube
		pMeshManager->m_LoadModelFromFile_Threaded("cube_flat_shaded_xyz_n_uv.ply");

		//tree bottoms
		pMeshManager->m_LoadModelFromFile_Threaded("tree_base1.ply");
		pMeshManager->m_LoadModelFromFile_Threaded("tree_base2.ply");

		//tree tops
		pMeshManager->m_LoadModelFromFile_Threaded("tree_top1.ply");
		pMeshManager->m_LoadModelFromFile_Threaded("tree_top2.ply");
		pMeshManager->m_LoadModelFromFile_Threaded("tree_top3.ply");
		pMeshManager->m_LoadModelFromFile_Threaded("tree_top4.ply");

		//tv
		pMeshManager->m_LoadModelFromFile_Threaded("RetroTV.edited.bodyonly.ply");
	}
	else
	{
		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "flat.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load flat.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load cube_flat_shaded_xyz_n_uv.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load Sphere_320_faces_xyz_n_GARBAGE_uv.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "lucy.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load lucy.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "tajmahal.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load tajmahal.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "dalek2005_xyz_uv_hi_res.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load dalek2005_xyz_uv_hi_res.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "dalek2005_xyz_uv_res_2.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load dalek2005_xyz_uv_res_2.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "dalek2005_xyz_uv_res_3.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load dalek2005_xyz_uv_res_3.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		//buildings
		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "b1.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load b1.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		//tv
		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "RetroTV.edited.bodyonly.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load RetroTV.edited.bodyonly.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		//capsule
		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "capsule.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load capsule.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		//cone
		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "cone.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load cone.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}

		//cube
		{
			sModelDrawInfo modelInfo;
			modelInfo.meshFileName = "cube_flat_shaded_xyz_n_uv.ply";
			if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId))
			{
				std::cout << "Didn't load cube_flat_shaded_xyz_n_uv.ply" << std::endl;
				std::cout << pMeshManager->GetLastError() << std::endl;
			}
		}
	}

	end_time = glfwGetTime();

	std::cout << "\t\t\tmodel load time on main thread: " << (end_time - start_time) << std::endl;
}