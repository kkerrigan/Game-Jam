#ifndef _cComponent_HG_
#define _cComponent_HG_

class cComponent {
public:
	const int COMPONENT_TYPE_ID;

	cComponent(int type) : COMPONENT_TYPE_ID(type) {}
	virtual ~cComponent() {};
};

#endif
