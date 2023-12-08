#pragma once
#include <map>
#include <string>
#include "audio/SoundDevice.h"
#include "audio/SoundBuffer.h"
#include "audio/SoundSource.h"

class AudioManager {
public:
	AudioManager();
	~AudioManager();
	void setDevicePosition(glm::vec3 camPos);
	void setDeviceOrientation(glm::vec3 trueFwd, glm::vec3 trueUp);
	uint32_t* addSoundEffect(std::string key, std::string fileName);
	uint32_t* getSoundEffect(std::string key);

private:
	SoundDevice* soundDevice;
	std::map<std::string, uint32_t> sounds;
};