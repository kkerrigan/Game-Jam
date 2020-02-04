

#include "cCommandGroup.h"

cCommandGroup::cCommandGroup(const std::string& name, bool isParallel) : finished(false), name(name), runParallel(isParallel)
{
}

cCommandGroup::~cCommandGroup()
{
}

void cCommandGroup::update(double deltaTime)
{
	bool areParallelCommandsDone = true;
	bool areSerialCommandsDone = true;

	//commands are executed in parallel
	for(std::vector<iCommand*>::iterator commandIt = this->commands.begin(); commandIt != this->commands.end(); ++commandIt)
	{
		iCommand* command = *commandIt;

		if(!command->isFinished())
		{
			areParallelCommandsDone = false;
			command->update(deltaTime);
		}
	}

	//groups are executed one at a time
	bool firstSerial = false;
	for(std::vector<cCommandGroup*>::iterator commandGroupIt = this->commandGroups.begin(); commandGroupIt != this->commandGroups.end(); ++commandGroupIt)
	{
		cCommandGroup* commandGroup = *commandGroupIt;

		if(commandGroup->isFinished())
		{
			continue;
		}

		//serial command group
		if(!commandGroup->runParallel && !firstSerial)
		{
			firstSerial = true;
			commandGroup->update(deltaTime);
			areSerialCommandsDone = false;
		}

		else if(commandGroup->runParallel)
		{
			commandGroup->update(deltaTime);
			areParallelCommandsDone = false;
		}
	}

	//update the is done
	this->finished = areParallelCommandsDone && areSerialCommandsDone;
}

bool cCommandGroup::isFinished() const
{
	return this->finished;
}
