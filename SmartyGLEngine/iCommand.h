
#pragma once

class iCommand
{
public:
	virtual ~iCommand() = default;
	virtual void update(float deltaTime) = 0;
	virtual bool isFinished() = 0;
};

