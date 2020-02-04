#ifndef _cComponentFactory_HG_
#define _cComponentFactory_HG_

#include <vector>

#include "cComponent.h"

class cComponentFactory {
public:
	static cComponentFactory* getInstance();
	cComponentFactory(cComponentFactory const&) = delete;
	void operator=(cComponentFactory const&) = delete;

	cComponent* createComponent(int componentType);

private:
	std::vector<cComponent*> allocatedComponents;

	cComponentFactory();
	~cComponentFactory();

};

#endif