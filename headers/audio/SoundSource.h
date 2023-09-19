#pragma once
#include <OpenAL\al.h>
#include <glm/glm.hpp>
class SoundSource
{
public:
	
	SoundSource(float p_Pitch, float p_Gain, glm::vec3 p_Position, glm::vec3 p_Velocity, bool p_LoopSound);
	
	~SoundSource();

	void Play(const ALuint buffer_to_play);

	void Stop();

	void Pause();

	void SetPosition(glm::vec3 pos);

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
};
