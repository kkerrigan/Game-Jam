
#pragma once

#include <string>
#include <vector>
#include "iCommand.h"

class cCommandGroup
{
	bool finished;
public:
	cCommandGroup(const std::string& name, bool isParallel);
	~cCommandGroup();
	void update(double deltaTime);
	bool isFinished() const;

	//contained commands and command groups
	std::vector<iCommand*> commands;
	std::vector<cCommandGroup*> commandGroups;

	std::string name;
	bool runParallel;
};

