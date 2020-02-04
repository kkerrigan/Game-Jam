/*
*
*	Abstract Component Builder
*
*	This creates entities with there components setup.
*/

#ifndef _cEntityBuilder_HG_
#define _cEntityBuilder_HG_

#include <vector>

#include "cEntity.h"

class cEntityBuilder {
public:
	static cEntityBuilder* getInstance();
	cEntityBuilder(cEntityBuilder const&) = delete;
	void operator=(cEntityBuilder const&) = delete;

	cEntity* createEntity(int entityType);
	cEntity* createEntity(int entityType, glm::vec3 position);
	cEntity* createEntityRandomPosColor(int entityType);

	template<typename T>
	static T getRandBetween0and1(void);

	template<typename T>
	static T getRandInRange(T min, T max);

private:
	std::vector<cEntity*> allocatedEntites;
	void addRequiredComponents(cEntity* entity, int entityType);
	cEntityBuilder();
	~cEntityBuilder();

	void setEntityPosition(cEntity* entity, glm::vec3 position);

};

#endif
