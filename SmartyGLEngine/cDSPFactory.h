#ifndef _cDSPFactory_HG_
#define _cDSPFactory_HG_

#include <fmod.hpp>

class cDSPFactory {
public:
	cDSPFactory(cDSPFactory const&) = delete;
	void operator=(cDSPFactory const&) = delete;
	
	static cDSPFactory* getInstance()
	{
		static cDSPFactory instance;

		return &instance;
	}
	
	FMOD::DSP* createDSP(int dspType);

private:
	cDSPFactory();
	~cDSPFactory();
};

#endif
