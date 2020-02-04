#pragma once

#include <Windows.h>		// For CRITICAL_SECTION, CreateThread, ResumeThread, etc.
#include <process.h>
#include "Ply/CHRTimer.h"
#include "cEntity.h"

class cScene;
// Forward declare 
class cDalekManagerTripleBuffer;
class cRandThreaded;

/*!	\class	cThreadWorkerComponent
	\brief
	\author Daniel Maclam
	\date	2019-03-01 4:45 AM
	*/
class cThreadWorkerComponent : public cComponent
{
public:
	static CRITICAL_SECTION should_run_critical_section;

	cThreadWorkerComponent();

	~cThreadWorkerComponent();

	int random_axis_aligned_direction(cEntity* entity, cScene* scene);
	int max_dalek_movment_down_hall(glm::vec3 position, int direction, int count);

	void start_thread(cEntity* entity);

	// The entire update of whatever it is that the Dalek
	//	is trying to (move, look at things in the world)
	void Update(void);

	void ResetTimerAndStart(void);

	void pick_new_location();

	static void stop_threads();
	static bool get_should_run();

	static void init_critical_section();
	static void delete_critical_section();

private:

	cEntity* entity;
	cRandThreaded* m_pRandThread;

	HANDLE hThreadHandle;

	glm::vec3 m_Velocity;
	CHRTimer* m_pLocalTimer;

	glm::vec3 target_location;

	static bool should_run;
};

