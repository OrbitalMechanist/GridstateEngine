#pragma once
#include <OpenAL\alc.h>
#include <glm/glm.hpp>

//Gets sound device to play sound from
class SoundDevice
{
public:
	static SoundDevice* get();

	void SetPosition(glm::vec3 pos);

	void SetOrientation(glm::vec3 fwd, glm::vec3 top);

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

};

