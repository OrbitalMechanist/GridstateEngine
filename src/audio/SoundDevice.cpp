/*
* Credits:
* https://www.youtube.com/watch?v=kWQM1iQ1W0E
* For providing a basic OpenAL tutorial on setting up and playing sounds
*/
#include "audio/SoundDevice.h"
#include <OpenAL\al.h>
#include <stdio.h>

//Get instance of sound device.
SoundDevice* SoundDevice::get()
{
	static SoundDevice* snd_device = new SoundDevice();
	return snd_device;
}

//Gets the sound devices in which audio will be played through and sets properties for audio listener
SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // Gets device, nullptr -> get default device
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
	if(!p_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	//printf("Opened \"%s\"\n", name);
}

//Closes sound device when destroyed
SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");

	alcDestroyContext(p_ALCContext);
	if (p_ALCContext)
		throw("failed to unset during close");

	if (!alcCloseDevice(p_ALCDevice))
		throw("failed to close sound device");
}

//Sets position of audio listener
void SoundDevice::SetPosition(glm::vec3 pos) {
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

//Sets orientation of audio listener
void SoundDevice::SetOrientation(glm::vec3 fwd, glm::vec3 top) {
	float o[] = { 
		fwd.x, fwd.y, fwd.z, 
		top.x, top.y, top.z 
	};
	alListenerfv(AL_ORIENTATION, o);
}
