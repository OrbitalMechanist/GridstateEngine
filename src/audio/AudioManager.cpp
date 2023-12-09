#include "audio/AudioManager.h"

AudioManager::AudioManager()
{
	// Sets up soundDevice
	soundDevice = SoundDevice::get();
	// Add sound effects
	addSoundEffect("spellHit", "spellHit.wav");
	addSoundEffect("injured", "injured.wav");
	addSoundEffect("meleeHit", "meleeHit.wav");
	addSoundEffect("select", "select.wav");
	addSoundEffect("invalid", "invalid.wav");
	addSoundEffect("move", "move.wav");
	addSoundEffect("buttonClick", "buttonClick.wav");
}

AudioManager::~AudioManager()
{
}

// Changes listening coordinates of sound device
void AudioManager::setDevicePosition(glm::vec3 camPos)
{
	soundDevice->SetPosition(camPos);
}

// Changes listening orientation of sound device
void AudioManager::setDeviceOrientation(glm::vec3 trueFwd, glm::vec3 trueUp)
{
	soundDevice->SetOrientation(trueFwd, trueUp);
}

// Adds a sound effect that can be accessed by a specific key
uint32_t AudioManager::addSoundEffect(std::string key, std::string fileName)
{
	if (!sounds.contains(key)) {
		sounds.insert(std::pair<std::string, uint32_t>(key, SoundBuffer::get()->addSoundEffect(("assets/audio/" + fileName).c_str())));
	}
	return getSoundEffect(key);
}

// Gets the sound effect of the specified key
uint32_t AudioManager::getSoundEffect(std::string key)
{
	if (sounds.contains(key)) {
	uint32_t s = sounds.find(key)->second;
	return s;
	}
	else {
		return NULL;
	}
}
