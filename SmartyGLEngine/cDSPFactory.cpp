#include "cDSPFactory.h"
#include "cSoundManager.h"


cDSPFactory::cDSPFactory() {}

cDSPFactory::~cDSPFactory() {}

FMOD::DSP* cDSPFactory::createDSP(int dspType) {

	FMOD::DSP* dspEffect = nullptr;
	cSoundManager* pSoundManager = cSoundManager::getInstance();

	switch (dspType) {
	case 0:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspEffect);
		pSoundManager->checkError();
		break;
	case 1:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dspEffect);
		pSoundManager->checkError();
		break;
	case 2:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dspEffect);
		pSoundManager->checkError();
		break;
	case 3:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspEffect);
		pSoundManager->checkError();
		break;
	case 4:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_LIMITER, &dspEffect);
		pSoundManager->checkError();
		break;
	case 5:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dspEffect);
		pSoundManager->checkError();
		break;
	case 6:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_CHORUS, &dspEffect);
		pSoundManager->checkError();
		break;
	case 7:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &dspEffect);
		pSoundManager->checkError();
		break;
	case 8:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_PAN, &dspEffect);
		pSoundManager->checkError();
		break;
	case 9:
		pSoundManager->mresult = pSoundManager->msystem->createDSPByType(FMOD_DSP_TYPE_FADER, &dspEffect);
		pSoundManager->checkError();
		break;
	}

	// if incompatible type passed in, return nullptr
	return dspEffect;
}