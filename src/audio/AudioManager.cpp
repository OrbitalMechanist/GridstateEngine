#include "audio/AudioManager.h"

AudioManager::AudioManager()
{
	soundDevice = SoundDevice::get();
}

AudioManager::~AudioManager()
{
}

void AudioManager::setDevicePosition(glm::vec3 camPos)
{
	soundDevice->SetPosition(camPos);
}

void AudioManager::setDeviceOrientation(glm::vec3 trueFwd, glm::vec3 trueUp)
{
	soundDevice->SetOrientation(trueFwd, trueUp);
}

uint32_t* AudioManager::addSoundEffect(std::string key, std::string fileName)
{
	if (getSoundEffect(key) == NULL) {
		sounds.insert(std::pair<std::string, uint32_t>(key, SoundBuffer::get()->addSoundEffect(("assets/audio/" + fileName).c_str())));
	}
	return getSoundEffect(key);
}

uint32_t* AudioManager::getSoundEffect(std::string key)
{
	uint32_t s = sounds.find(key)->second;
	return &s;
}
