#include "cConsole.h"
#include "cSceneManager.h"
#include "cSoundManager.h"
#include "cDSPFactory.h"

FMOD_RESULT F_CALLBACK endOfSound(FMOD_CHANNELCONTROL*channelControl, FMOD_CHANNELCONTROL_TYPE controlType,
	FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void*commanData1, void*commanData2)
{
	if (controlType == FMOD_CHANNELCONTROL_CHANNEL && callbackType == FMOD_CHANNELCONTROL_CALLBACK_END) {
		FMOD::Channel* channel = (FMOD::Channel*)channelControl;
		
		cSceneManager* sceneManager = cSceneManager::getInstance();
		cScene* scene = sceneManager->getActiveScene();

		std::vector<cSoundManager::sSoundInfo*> sounds = scene->getSounds();

		for (unsigned int i = 0; i < sounds.size(); ++i) {
			if (sounds[i]->channel == channel) {
				sounds[i]->channel = nullptr;
			}
		}
	}

	return FMOD_OK;
}

bool cSoundManager::initialize() {
	mresult = FMOD::System_Create(&this->msystem);
	if (this->checkError()) {
		return false;
	}

	mresult = this->msystem->init(512, FMOD_INIT_NORMAL, 0);
	if (this->checkError()) {
		return false;
	}

	mresult = msystem->getMasterChannelGroup(&this->channel_groups[0]);
	this->checkError();

	// create all the other channel groups 
	for (int i = 1; i < NUM_OF_CHANNEL_GROUPS; ++i) {
		mresult = this->msystem->createChannelGroup(nullptr, &this->channel_groups[i]);
		this->checkError();

		mresult = this->channel_groups[0]->addGroup(this->channel_groups[i]);
		this->checkError();
	}

	//load the dps effects
	for (int i = 0; i < 9; ++i) {
		this->effects[i] = cDSPFactory::getInstance()->createDSP(i);
		this->effects[i]->setBypass(true);
	}

	//add the effects to the appropriate channel groups
	this->channel_groups[1]->addDSP(0, this->effects[0]);
	this->channel_groups[1]->addDSP(0, this->effects[1]);
	this->channel_groups[1]->addDSP(0, this->effects[2]);

	this->channel_groups[2]->addDSP(0, this->effects[3]);
	this->channel_groups[2]->addDSP(0, this->effects[4]);
	this->channel_groups[2]->addDSP(0, this->effects[5]);

	this->channel_groups[3]->addDSP(0, this->effects[6]);
	this->channel_groups[3]->addDSP(0, this->effects[7]);
	this->channel_groups[3]->addDSP(0, this->effects[8]);

	return true;
}

bool cSoundManager::load3dSound(sSoundInfo* soundInfo, FMOD_MODE modes) {
	soundInfo->type = eSoundType::SAMPLE;
	mresult = this->msystem->getChannel(this->sounds.size() + 1, &soundInfo->channel);
	if (this->checkError()) {
		return false;
	}

	mresult = this->msystem->createSound(std::string(this->basePath + soundInfo->fileName).c_str(), FMOD_3D | FMOD_3D_LINEARROLLOFF | modes, 0, &soundInfo->sound);
	if (this->checkError()) {
		return false;
	}

	this->sounds.push_back(soundInfo);

	return true;
}

bool cSoundManager::loadSample(sSoundInfo* soundInfo, FMOD_MODE mode) {
	soundInfo->type = eSoundType::SAMPLE;
	mresult = this->msystem->getChannel(this->sounds.size() + 1, &soundInfo->channel);
	if (this->checkError()) {
		return false;
	}

	mresult = this->msystem->createSound(std::string(this->basePath + soundInfo->fileName).c_str(), FMOD_DEFAULT | mode, 0, &soundInfo->sound);
	if (this->checkError()) {
		return false;
	}

	this->sounds.push_back(soundInfo);

	return true;
}

bool cSoundManager::loadStream(sSoundInfo* soundInfo) {
	soundInfo->type = eSoundType::STREAM;
	mresult = this->msystem->getChannel(this->sounds.size() + 1, &soundInfo->channel);
	if (this->checkError()) {
		return false;
	}

	mresult = this->msystem->createStream(std::string(this->basePath + soundInfo->fileName).c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &soundInfo->sound);
	if (this->checkError()) {
		return false;
	}

	this->sounds.push_back(soundInfo);

	return true;
}

bool cSoundManager::playSound(sSoundInfo* soundInfo) {
	mresult = this->msystem->playSound(soundInfo->sound, this->channel_groups[soundInfo->channelGroupIndex], false, &soundInfo->channel);
	
	soundInfo->channel->setCallback(endOfSound);

	if (this->checkError()) {
		return false;
	}

	return true;
}

void cSoundManager::shutDown() {
	//TODO: release all the sounds
	for (sSoundInfo* sound : this->sounds) {
		mresult = sound->sound->release();
		if (mresult != FMOD_OK)
		{
			fprintf(stderr, "FMOD error! (%d) %s\n", mresult, FMOD_ErrorString(mresult));
			exit(-1);
		}
	}

	mresult = this->msystem->close();
	if (mresult != FMOD_OK)
	{
		fprintf(stderr, "FMOD error! (%d) %s\n", mresult, FMOD_ErrorString(mresult));
		exit(-2);
	}

	mresult = this->msystem->release();
	if (mresult != FMOD_OK)
	{
		fprintf(stderr, "FMOD error! (%d) %s\n", mresult, FMOD_ErrorString(mresult));
		exit(-3);
	}
}

cSoundManager::sSoundInfo* cSoundManager::getSoundInfoByIndex(unsigned int index) {
	return this->sounds[index];
}

void cSoundManager::getSoundFormat(sSoundInfo& soundInfo) {

	if (soundInfo.sound != nullptr) {
		this->mresult = soundInfo.sound->getFormat(0, &soundInfo.format, 0, 0);
		this->checkError();
	}
}

void cSoundManager::getSoundFrequency(sSoundInfo& soundInfo) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->getFrequency(&soundInfo.frequency);
		this->checkError();
	}

}

void cSoundManager::setSoundFrequency(sSoundInfo& soundInfo, float frequency) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->setFrequency(frequency);
		this->checkError();
	}
}

void cSoundManager::getSoundVolume(sSoundInfo& soundInfo) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->getVolume(&soundInfo.volume);
		this->checkError();
	}
}

void cSoundManager::setSoundVolume(sSoundInfo& soundInfo, float volume) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->setVolume(volume);
		this->checkError();
	}
}

float cSoundManager::getSoundBalance(sSoundInfo& soundInfo) {
	return soundInfo.balance;
}

void cSoundManager::setSoundBalance(sSoundInfo& soundInfo, float balance) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->setPan(balance);
		this->checkError();
	}
}

void cSoundManager::getSoundCurrentPosition(sSoundInfo& soundInfo) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->getPosition(&soundInfo.channelPosition, FMOD_TIMEUNIT_MS);
		this->checkError();

		if (soundInfo.sound != nullptr) {
			if (soundInfo.sound) {
				this->mresult = soundInfo.sound->getLength(&soundInfo.soundLength, FMOD_TIMEUNIT_MS);
				this->checkError();
			}
		}
	}
}


void cSoundManager::getIsPaused(sSoundInfo& soundInfo) {
	this->mresult = soundInfo.channel->getPaused(&soundInfo.isPaused);
	this->checkError();
}

void cSoundManager::getSoundPitch(sSoundInfo& soundInfo) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->getPitch(&soundInfo.pitch);
		this->checkError();
	}
}

void cSoundManager::setSoundPitch(sSoundInfo& soundInfo, float pitch) {
	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->setPitch(pitch);
		this->checkError();
	}
}

void cSoundManager::setPaused(sSoundInfo* soundInfo, bool isPaused) {
	soundInfo->channel->setPaused(isPaused);
}

void cSoundManager::setBasePath(std::string base) {
	this->basePath = base;
}

bool cSoundManager::checkError() {
	bool result = true;
	if (mresult != FMOD_OK) {
		this->getErrorMessage(this->mresult);
	}
	else {
		result = false;
	}

	// clear the error
	this->mresult = FMOD_OK;

	return result;
}

void cSoundManager::setSoundCurrentPosition(sSoundInfo& soundInfo, unsigned int position) {

	soundInfo.channelPosition = position;

	if (soundInfo.channel != nullptr) {
		this->mresult = soundInfo.channel->setPosition(soundInfo.channelPosition, FMOD_TIMEUNIT_MS);
		this->checkError();
	}
}

void cSoundManager::update() {

	this->mresult = this->msystem->set3DListenerAttributes(0, &listener_position, &channel_velocity, &forward, &up);

	this->checkError();

	mresult = msystem->update();
	this->checkError();
}

void cSoundManager::sSoundInfo::serializeSound(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	cSoundManager* pSoundManager = cSoundManager::getInstance();
	pSoundManager->getIsPaused(*this);
	pSoundManager->getSoundFrequency(*this);
	pSoundManager->getSoundVolume(*this);
	pSoundManager->getSoundBalance(*this);
	pSoundManager->getSoundBalance(*this);
	pSoundManager->getSoundPitch(*this);
	pSoundManager->getSoundCurrentPosition(*this);

	writer.String("fileName");
	writer.String(this->fileName.c_str());

	writer.String("soundType");
	writer.Int(this->type);

	writer.String("channelGroup");
	writer.Int(this->channelGroupIndex);

	writer.String("isPaused");
	writer.Bool(this->isPaused);

	writer.String("isCompressed");
	writer.Bool(this->isCompressed);

	writer.String("frequency");
	writer.Double(this->frequency);

	writer.String("volume");
	writer.Double(this->volume);

	writer.String("balance");
	writer.Double(this->balance);

	writer.String("pitch");
	writer.Double(this->pitch);

	writer.String("soundLength");
	writer.Uint(this->soundLength);

	writer.String("channelPosition");
	writer.Uint(this->channelPosition);

	writer.String("minDistance");
	writer.Double(this->f3dMinDistance);

	writer.String("maxDistance");
	writer.Double(this->f3dMaxDistance);
}

void cSoundManager::getErrorMessage(int errorCode) {

	switch (errorCode) {
	case FMOD_ERR_BADCOMMAND:
		std::cout << "\t\t\tFMOD Error: Tried to call a function on a data type that does not allow this type of functionality" << std::endl;
		break;
	case FMOD_ERR_CHANNEL_ALLOC:
		std::cout << "\t\t\tFMOD Error trying to allocate a channel." << std::endl;
		break;
	case FMOD_ERR_CHANNEL_STOLEN:
		std::cout << "\t\t\tFMOD Error: The specified channel has been reused to play another sound." << std::endl;
		break;
	case FMOD_ERR_DSP_CONNECTION :
		std::cout << "\t\t\tFMOD Error: DSP connection error. Connection possibly caused a cyclic dependency or connected dsps with incompatible buffer counts." << std::endl;
	case FMOD_ERR_DSP_DONTPROCESS:
		std::cout << "\t\t\tFMOD Error: DSP return code from a DSP process query callback. Tells mixer not to call the process callback and therefore not consume CPU. Use this to optimize the DSP graph." << std::endl;
		break;
	case FMOD_ERR_DSP_FORMAT:
		std::cout << "\t\t\tFMOD Error: DSP Format error. A DSP unit may have attempted to connect to this network with the wrong format, or a matrix may have been set with the wrong size if the target unit has a specified channel map." << std::endl;
		break;
	case FMOD_ERR_DSP_INUSE:
		std::cout << "\t\t\tFMOD Error: DSP is already in the mixer's DSP network. It must be removed before being reinserted or released." << std::endl;
		break;
	case FMOD_ERR_DSP_NOTFOUND:
		std::cout << "\t\t\tFMOD Error: DSP connection error. Couldn't find the DSP unit specified." << std::endl;
		break;
	case FMOD_ERR_DSP_RESERVED:
		std::cout << "\t\t\tFMOD Error: DSP operation error. Cannot perform operation on this DSP as it is reserved by the system." << std::endl;
		break;
	case FMOD_ERR_DSP_SILENCE:
		std::cout << "\t\t\tFMOD Error: DSP return code from a DSP process query callback. Tells mixer silence would be produced from read, so go idle and not consume CPU. Use this to optimize the DSP graph." << std::endl;
		break;
	case FMOD_ERR_DSP_TYPE:
		std::cout << "\t\t\tFMOD Error: DSP operation cannot be performed on a DSP of this type." << std::endl;
		break;
	case FMOD_ERR_FILE_BAD:
		std::cout << "\t\t\tFMOD Error loading file." << std::endl;
		break;
	case FMOD_ERR_FILE_COULDNOTSEEK:
		std::cout << "\t\t\tFMOD Error: Couldn't perform seek operation. This is a limitation of the medium (ie netstreams) or the file format." << std::endl;
		break;
	case FMOD_ERR_INVALID_HANDLE:
		std::cout << "\t\t\tFMOD Error: An invalid object handle was used." << std::endl;
		break;
	case FMOD_ERR_FILE_EOF:
		std::cout << "\t\t\tFMOD Error: End of file unexpectedly reached while trying to read essential data (truncated?)." << std::endl;
		break;
	case FMOD_ERR_FILE_ENDOFDATA:
		std::cout << "\t\t\tFMOD Error: End of current chunk reached while trying to read data." << std::endl;
		break;
	case FMOD_ERR_FILE_NOTFOUND:
		std::cout << "\t\t\tFMOD Error: File not found." << std::endl;
		break;
	case FMOD_ERR_INITIALIZATION:
		std::cout << "\t\t\tFMOD Error: FMOD was not initialized correctly to support this function." << std::endl;
		break;
	case FMOD_ERR_INITIALIZED:
		std::cout << "\t\t\tFMOD Error: Cannot call this command after System::init." << std::endl;
		break;
	case FMOD_ERR_INTERNAL:
		std::cout << "\t\t\tFMOD Error: An error occurred that wasn't supposed to. Contact support." << std::endl;
		break;
	case FMOD_ERR_INVALID_FLOAT:
		std::cout << "\t\t\tFMOD Error: Value passed in was a NaN, Inf or denormalized float." << std::endl;
		break;
	case FMOD_ERR_INVALID_PARAM:
		std::cout << "\t\t\tFMOD Error: An invalid parameter was passed to this function." << std::endl;
		break;
	case FMOD_ERR_INVALID_POSITION:
		std::cout << "\t\t\tFMOD Error: An invalid seek position was passed to this function." << std::endl;
		break;
	case FMOD_ERR_INVALID_THREAD:
		std::cout << "\t\t\tFMOD Error: Tried to call a function on a thread that is not supported." << std::endl;
		break;
	case FMOD_ERR_MEMORY:
		std::cout << "\t\t\tFMOD Error: Not enough memory or resources." << std::endl;
		break;
	case FMOD_ERR_UNIMPLEMENTED:
		std::cout << "\t\t\tFMOD Error: Something in FMOD hasn't been implemented when it should be! contact support!" << std::endl;
		break;
	case FMOD_ERR_UNINITIALIZED:
		std::cout << "\t\t\tFMOD Error: This command failed because System::init or System::setDriver was not called." << std::endl;
		break;
	case FMOD_ERR_UNSUPPORTED:
		std::cout << "\t\t\tFMOD Error: A command issued was not supported by this object. Possibly a plugin without certain callbacks specified." << std::endl;
		break;
	default:
		std::cout << "\t\t\tFMOD Error Code " << errorCode << " consult documentation to find out more." << std::endl;
		break;
	}
}