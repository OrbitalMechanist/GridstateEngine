#include "audio/SoundSource.h"
#include <iostream>

SoundSource::SoundSource(float p_Pitch, float p_Gain, glm::vec3 p_Position, glm::vec3 p_Velocity, bool p_LoopSound)
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position.x, p_Position.y, p_Position.z);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity.x, p_Velocity.y, p_Velocity.z);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &p_Source);
}

void SoundSource::Play(const ALuint buffer_to_play)
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING) {
		alSourceStop(p_Source);
	}
	if (buffer_to_play != p_Buffer)
	{
			p_Buffer = buffer_to_play;
			alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}

	alSourcePlay(p_Source);
}

void SoundSource::Pause()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING) {
		alSourcePause(p_Source);
	}
}

void SoundSource::Stop()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING) {
		alSourceStop(p_Source);
	}
}

void SoundSource::SetPosition(glm::vec3 pos) {
	alSource3f(p_Source, AL_POSITION, pos.x, pos.y, pos.z);
}
