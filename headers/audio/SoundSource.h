#pragma once
#include <OpenAL\al.h>
#include <OpenAL\alext.h>
#include <glm/glm.hpp>
class SoundSource
{
public:
	
	SoundSource(float p_Pitch, float p_Gain, glm::vec3 p_Position, glm::vec3 p_Velocity, bool p_LoopSound, bool positionalAudio);
	
	~SoundSource();

	void Play(const ALuint buffer_to_play);

	void Stop();

	void Pause();

	void SetPitch(float pitch);

	void SetGain(float gain);

	void SetPosition(glm::vec3 pos);

	void SetVelocity(glm::vec3 vel);

	void SetLooping(bool looping);

	float GetPitch();
	
	float GetGain();

	glm::vec3 GetPosition();

	glm::vec3 GetVelocity();

	void SetPositionalAudio(bool positionalAudio);

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
};

