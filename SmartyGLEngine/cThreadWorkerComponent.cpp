/*
* Name:		cThreadWorkerComponent.h
* Author:	Daniel Maclam
* Date		2019-03-01 4:45 AM
*/

#include "cThreadWorkerComponent.h"
#include "cMazeHelpers.h"
#include "cScene.h"
#include "cSceneManager.h"

//Gems - Question #3 - This just calles the update function on the cThreadWorkerComponent
DWORD WINAPI DalekBrainThread(void* pInitialData)
{
	cEntity* pDalek = (cEntity*)pInitialData;

	while (cThreadWorkerComponent::get_should_run())
	{
		if (pDalek)
		{
			cThreadWorkerComponent* worker = static_cast<cThreadWorkerComponent*>(pDalek->components[11]);

			if (worker)
			{
				worker->Update();
			}
		}

		// Release control of the thread
		Sleep(1);
	}

	return 0;
}

 bool cThreadWorkerComponent::should_run = true;

//Gems - Question #3 - Each dalek has this component that and the threads run the function above
void cThreadWorkerComponent::start_thread(cEntity* entity)
{
	static bool should_run;

	LPDWORD phThread = 0;	// Clear to zero
	DWORD hThread = 0;
	HANDLE hThreadHandle = 0;

	
	void* pThisDalek = (void*)(entity);
	this->entity = entity;

	std::cout << "Creating Dalek thread..." << std::endl;
	std::cout.flush();

	cTransformComponent* transform = static_cast<cTransformComponent*>(this->entity->components[1]);

	switch (this->random_axis_aligned_direction(this->entity, cSceneManager::getInstance()->getActiveScene()))
	{
	case 0:
		this->target_location = transform->get_position() + glm::vec3(0.0f, 0.0f, 4.0f);
		break;

	case 1:
		this->target_location = transform->get_position() + glm::vec3(4.0f, 0.0f, -.0f);
		break;

	case 2:
		this->target_location = transform->get_position() + glm::vec3(0.0f, 0.0f, -4.0f);
		break;

	case 3:
		this->target_location = transform->get_position() + glm::vec3(-4.0f, 0.0f, 0.0f);
		break;
	}

	hThreadHandle = CreateThread(NULL,	// Default security
		0,		// Stack size - default - win32 = 1 Mbyte
		&DalekBrainThread, // Pointer to the thread's function
		pThisDalek,		// The value (parameter) we pass to the thread
			// This is a pointer to void... more on this evil thing later...
		0,  // CREATE_SUSPENDED or 0 for threads...
		(DWORD*)&phThread);		// pointer or ref to variable that will get loaded with threadID

}

cThreadWorkerComponent::cThreadWorkerComponent(): cComponent(11), entity(nullptr), m_pRandThread(nullptr),
                                                  m_Velocity(glm::vec3(0.0f)),
                                                  m_pLocalTimer(nullptr)
{
	this->m_pLocalTimer = new CHRTimer();
}

cThreadWorkerComponent::~cThreadWorkerComponent()
{
}

int cThreadWorkerComponent::random_axis_aligned_direction(cEntity* entity, cScene* scene)
{
	//Gems - Question #3 - pick a random directions
	int direction = rand() % 4;

	cTransformComponent* transform = static_cast<cTransformComponent*>(entity->components[1]);

	//see if we can go that way
	unsigned long long box_id;
	glm::vec3 newPos;
	switch (direction)
	{
	case 0: //+z
		newPos = transform->get_position() + glm::vec3(0.0f, 0.0f, 4.0f);
		break;

	case 1: //+x
		newPos = transform->get_position() + glm::vec3(4.0f, 0.0f, 0.0f);
		break;

	case 2: //-z
		newPos = transform->get_position() + glm::vec3(0.0f, 0.0f, -4.0f);
		break;

	case 3: //-x
		newPos = transform->get_position() + glm::vec3(-4.0f, 0.0f, 0.0f);
		break;
	}

	cMazeHelpers::find_closest_cube(scene->theMM.maze, newPos, &box_id);

	//if it's a wall then we cannot go that way.. so try again
	if (scene->maze_info[box_id].is_wall)
	{
		return random_axis_aligned_direction(entity, scene);
	}

	return direction;
}

int cThreadWorkerComponent::max_dalek_movment_down_hall(glm::vec3 position, int direction, int count = 0)
{
	//Gems - Question #3 - calculate how far we can move down the hallway in number of cubes

	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	glm::vec3 newPos;
	switch (direction)
	{
	case 0: //+z
		newPos = position + glm::vec3(0.0f, 0.0f, 4.0f);
		break;

	case 1: //+x
		newPos = position + glm::vec3(4.0f, 0.0f, 0.0f);
		break;

	case 2: //-z
		newPos = position + glm::vec3(0.0f, 0.0f, -4.0f);
		break;

	case 3: //-x
		newPos = position + glm::vec3(-4.0f, 0.0f, 0.0f);
		break;
	}

	unsigned long long box_id;
	if(!scene)
	{
		return 0;
	}

	cMazeHelpers::find_closest_cube(scene->theMM.maze, newPos, &box_id);

	if(scene->maze_info.find(box_id) == scene->maze_info.end())
	{
		return count;
	}

	if (scene->maze_info[box_id].is_wall)
	{
		return count;
	}else
	{
		++count;
		return this->max_dalek_movment_down_hall(newPos, direction, count);
	}
}


//Gems - Question #3 - all the real work for the thread is here
void cThreadWorkerComponent::Update()
{
	cEntity* entity = this->entity;
	cTransformComponent* transform = static_cast<cTransformComponent*>(this->entity->components[1]);

	//if we reached our destination look for a new one
	if (glm::abs(glm::distance(transform->get_position(), this->target_location)) < 0.1f)
	{
		this->pick_new_location();
		Sleep(2000); //wait a bit so we can see the dalek stopping at random distances in the halls
	}

	//Gems - Question #3 - the setter and getter for position and orientation are thread safe (check them out in the cTransformComponent)
	//move towards the target
	glm::vec3 direction = glm::normalize(this->target_location - transform->get_position());

	glm::vec3 pos = transform->get_position();
	glm::quat new_oreientation = glm::quat(glm::inverse(glm::lookAt(pos, pos - direction, glm::vec3(0.0f, 1.0f, 0.0f)))); //glm::vec3(0.0f,1.0f,0.0f)));
	transform->setQOrientation(new_oreientation);

	transform->set_position(transform->get_position() + direction * 0.01f);
}

void cThreadWorkerComponent::pick_new_location()
{

	cTransformComponent* transform = static_cast<cTransformComponent*>(this->entity->components[1]);

	int direction_to_go = this->random_axis_aligned_direction(this->entity, cSceneManager::getInstance()->getActiveScene());
	int max_squares = this->max_dalek_movment_down_hall(transform->get_position(), direction_to_go);
	int num_squares_to_move = rand() % max_squares + 1;

	switch (direction_to_go)
	{
	case 0:
		this->target_location = transform->get_position() + (glm::vec3(0.0f, 0.0f, 4.0f) * (float)num_squares_to_move);
		break;

	case 1:
		this->target_location = transform->get_position() + (glm::vec3(4.0f, 0.0f, -.0f) * (float)num_squares_to_move);
		break;

	case 2:
		this->target_location = transform->get_position() + (glm::vec3(0.0f, 0.0f, -4.0f) * (float)num_squares_to_move);
		break;

	case 3:
		this->target_location = transform->get_position() + (glm::vec3(-4.0f, 0.0f, 0.0f) * (float)num_squares_to_move);
		break;
	}
}

void cThreadWorkerComponent::stop_threads()
{
	EnterCriticalSection(&cThreadWorkerComponent::should_run_critical_section);
	cThreadWorkerComponent::should_run = false;
	LeaveCriticalSection(&cThreadWorkerComponent::should_run_critical_section);
}

bool cThreadWorkerComponent::get_should_run()
{
	EnterCriticalSection(&cThreadWorkerComponent::should_run_critical_section);
	bool should_run = cThreadWorkerComponent::should_run;
	LeaveCriticalSection(&cThreadWorkerComponent::should_run_critical_section);
	return should_run;
}

//Gems - Question #3 - CRITICAL SECTIONS TO PROTECT THE DATA (this is for the bool controlling the life of all the threads)
void cThreadWorkerComponent::init_critical_section()
{
	InitializeCriticalSection(&cThreadWorkerComponent::should_run_critical_section);
}

void cThreadWorkerComponent::delete_critical_section()
{
	DeleteCriticalSection(&cThreadWorkerComponent::should_run_critical_section);
}

CRITICAL_SECTION cThreadWorkerComponent::should_run_critical_section;
