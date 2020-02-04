#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_common.h>
#include <vector>
#include <map>

#include <rapidjson/prettywriter.h>

#define NUM_OF_CHANNEL_GROUPS 10
#define NUM_OF_EFFECTS 9

enum eSoundType {
	STREAM,
	SAMPLE
};

class cSoundManager
{
private:
	cSoundManager() : selectedSound(0) {}

public:

	struct sSoundInfo {
		std::string fileName;
		int type;
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		FMOD::ChannelGroup* channelGroup;
		FMOD_SOUND_FORMAT format;
		int channelGroupIndex;
		bool isPaused;
		bool isCompressed;
		float frequency;
		float volume;
		float balance;
		float pitch;
		unsigned soundLength;
		unsigned channelPosition;
		float f3dMinDistance;
		float f3dMaxDistance;

		sSoundInfo() : balance(0.0f), volume(0.0f), sound(nullptr), channel(nullptr), isCompressed(false), channelGroupIndex(0), f3dMinDistance(1.0f), f3dMaxDistance(10000.0f){};

		void serializeSound(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	};

	FMOD_VECTOR listener_position = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR channel_velocity = { 0.0f,0.0f,0.0f };

	cSoundManager(cSoundManager const&) = delete;
	void operator=(cSoundManager const&) = delete;

	FMOD::ChannelGroup* channel_groups[NUM_OF_CHANNEL_GROUPS];
	float channelGroupBalance[NUM_OF_CHANNEL_GROUPS] = { 0.0f };
	FMOD::DSP* effects[NUM_OF_EFFECTS];

	std::string basePath;
	std::vector<sSoundInfo*> sounds;
	
	unsigned selectedSound;
	FMOD_RESULT mresult = FMOD_OK;
	FMOD::System *msystem = nullptr;

	static cSoundManager* getInstance()
	{
		static cSoundManager instance;

		return &instance;
	}

	bool initialize();
	bool load3dSound(sSoundInfo* soundInfo, FMOD_MODE mode);
	bool loadSample(sSoundInfo* soundInfo, FMOD_MODE mode);
	bool loadStream(sSoundInfo* soundInfo);
	bool playSound(sSoundInfo* soundInfo);
	void update();
	void shutDown();

	sSoundInfo* getSoundInfoByIndex(unsigned int index);

	void getSoundFormat(sSoundInfo& soundInfo);

	void getSoundFrequency(sSoundInfo& soundInfo);
	void setSoundFrequency(sSoundInfo& soundInfo, float frequency);

	void getSoundVolume(sSoundInfo& soundInfo);
	void setSoundVolume(sSoundInfo& soundInfo, float volume);

	float getSoundBalance(sSoundInfo& soundInfo);
	void setSoundBalance(sSoundInfo& soundInfo, float balance);

	void getSoundCurrentPosition(sSoundInfo& soundInfo);
	void setSoundCurrentPosition(sSoundInfo& soundInfo, unsigned int position);

	void getIsPaused(sSoundInfo& soundInfo);
	void setPaused(sSoundInfo* soundInfo, bool isPaused);

	void getSoundPitch(sSoundInfo& soundInfo);
	void setSoundPitch(sSoundInfo& soundInfo, float pitch);

	void setBasePath(std::string base);

	void getErrorMessage(int errorCode);

	bool checkError();
};

