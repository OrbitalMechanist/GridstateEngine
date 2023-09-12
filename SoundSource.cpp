#include "SoundSource.h"
#include <iostream>

SoundSource::SoundSource(float p_Pitch, float p_Gain, glm::vec3 p_Position, glm::vec3 p_Velocity, bool p_LoopSound)
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position.x, p_Position.y, p_Position.z);
	alSource3f(p_Source, AL_VELOCITY, p_Position.x, p_Position.y, p_Position.z);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &p_Source);
}

void SoundSource::Play(const ALuint buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
			p_Buffer = buffer_to_play;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);

	ALint state = AL_PLAYING;
	std::cout << "playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	}
	std::cout << "done playing sound\n";
}
