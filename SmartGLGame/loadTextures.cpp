#include <cBasicTextureManager.h>
#include <iostream>

void loadTextures() {
	cBasicTextureManager* pBasicTextureManager = cBasicTextureManager::getInstance();
	pBasicTextureManager->SetBasePath("./assets/textures/");

	//pBasicTextureManager->Create2DTextureFromBMPFile("flames.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("snowflake.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("debug_grid.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("scooby.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("fur.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("hand.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("path.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("rocks.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("grass.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("water1.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("bark.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("leaves.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("smoke.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("fire.bmp", true);

	pBasicTextureManager->Create2DTextureFromBMPFile("island.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("island_normal.bmp", true);
	pBasicTextureManager->Create2DTextureFromBMPFile("island_splat.bmp", true);

	//pBasicTextureManager->Create2DTextureFromBMPFile("royal_carpet.bmp", true);
	//pBasicTextureManager->Create2DTextureFromBMPFile("target.bmp", true);
	// Load cube maps
	pBasicTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;

	pBasicTextureManager->SetBasePath("assets/textures/cubemaps/sb_night/");

	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("night_cube_map",
		"SkyMidNight_Right.bmp",
		"SkyMidNight_Left.bmp",		// Alternate these
		"SkyMidNight_Bottom.bmp",
		"SkyMidNight_Top.bmp", 			// Rotate these 90 degrees
		"SkyMidNight_Front.bmp",
		"SkyMidNight_Back.bmp",
		true, errorString))
	{
		std::cout << "Loaded the night cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: night cube map DIDN't load. On no!" << std::endl;
	}

	pBasicTextureManager->SetBasePath("assets/textures/cubemaps/sb_dusk/");
	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("dawn_cube_map",
		"SkyEarlyDusk_Right.bmp",
		"SkyEarlyDusk_Left.bmp",		// Alternate these
		"SkyEarlyDusk_Bottom.bmp",
		"SkyEarlyDusk_Top.bmp", 			// Rotate these 90 degrees
		"SkyEarlyDusk_Front.bmp",
		"SkyEarlyDusk_Back.bmp",
		true, errorString))
	{
		std::cout << "Loaded the dusk cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: dusk cube map DIDN't load. On no!" << std::endl;
	}

	pBasicTextureManager->SetBasePath("assets/textures/cubemaps/sb_morning/");
	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("morning_cube_map",
		"SkyMorning_Right.bmp",
		"SkyMorning_Left.bmp",		// Alternate these
		"SkyMorning_Bottom.bmp",
		"SkyMorning_Top.bmp", 			// Rotate these 90 degrees
		"SkyMorning_Front.bmp",
		"SkyMorning_Back.bmp",
		true, errorString))
	{
		std::cout << "Loaded the dusk cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: dusk cube map DIDN't load. On no!" << std::endl;
	}

	pBasicTextureManager->SetBasePath("assets/textures/cubemaps/sb_day/");
	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("day_cube_map",
		"SkyNoon_Right.bmp",
		"SkyNoon_Left.bmp",		// Alternate these
		"SkyNoon_Bottom.bmp",
		"SkyNoon_Top.bmp", 			// Rotate these 90 degrees
		"SkyNoon_Front.bmp",
		"SkyNoon_Back.bmp",
		true, errorString))
	{
		std::cout << "Loaded the day cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: day cube map DIDN't load. On no!" << std::endl;
	}

	pBasicTextureManager->SetBasePath("assets/textures/cubemaps/sb_sunset/");
	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("dusk_cube_map",
		"SkySunSet_Right.bmp",
		"SkySunSet_Left.bmp",		// Alternate these
		"SkySunSet_Bottom.bmp",
		"SkySunSet_Top.bmp", 			// Rotate these 90 degrees
		"SkySunSet_Front.bmp",
		"SkySunSet_Back.bmp",
		true, errorString))
	{
		std::cout << "Loaded the day cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: day cube map DIDN't load. On no!" << std::endl;
	}

	return;
}