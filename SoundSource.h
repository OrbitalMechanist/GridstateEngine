#pragma once
#include <OpenAL\al.h>
#include <glm/glm.hpp>
class SoundSource
{
public:
	
	SoundSource(float p_Pitch, float p_Gain, glm::vec3 p_Position, glm::vec3 p_Velocity, bool p_LoopSound);
	
	~SoundSource();

	void Play(const ALuint buffer_to_play);

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
	float p_Pitch;
	float p_Gain;
	float p_Position[3];
	float p_Velocity[3];
	bool p_LoopSound;
};

