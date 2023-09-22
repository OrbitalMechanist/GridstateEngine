#include "audio/SoundSource.h"
#include <iostream>

SoundSource::SoundSource(float p_Pitch, float p_Gain, glm::vec3 p_Position, glm::vec3 p_Velocity, bool p_LoopSound,  bool positionalAudio)
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position.x, p_Position.y, p_Position.z);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity.x, p_Velocity.y, p_Velocity.z);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	if (positionalAudio) {
		alSourcei(p_Source, AL_SOURCE_SPATIALIZE_SOFT, AL_TRUE);
	}
	else {
		alSourcei(p_Source, AL_SOURCE_SPATIALIZE_SOFT, AL_FALSE);
	}
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

void SoundSource::SetPitch(float pitch) {
	alSourcef(p_Source, AL_PITCH, pitch);
}

void SoundSource::SetGain(float gain) {
	alSourcef(p_Source, AL_GAIN, gain);
}

void SoundSource::SetPosition(glm::vec3 pos) {
	alSource3f(p_Source, AL_POSITION, pos.x, pos.y, pos.z);
}

void SoundSource::SetVelocity(glm::vec3 vel) {
	alSource3f(p_Source, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void SoundSource::SetLooping(bool looping) {
	alSourcei(p_Source, AL_LOOPING, looping);
}

float SoundSource::GetPitch() {
	float f;
	alGetSourcef(p_Source, AL_PITCH, &f);
	return f;
}

float SoundSource::GetGain() {
	float g;
	alGetSourcef(p_Source, AL_GAIN, &g);
	return g;
}

glm::vec3 SoundSource::GetPosition() {
	float x; float y; float z;
	alGetSource3f(p_Source, AL_POSITION, &x, &y, &z);
	return { x, y, z };
}

glm::vec3 SoundSource::GetVelocity() {
	float x; float y; float z;
	alGetSource3f(p_Source, AL_VELOCITY, &x, &y, &z);
	return { x, y, z };
}

void SoundSource::SetPositionalAudio(bool positionalAudio) {
	if (positionalAudio) {
		alSourcei(p_Source, AL_SOURCE_SPATIALIZE_SOFT, AL_TRUE);
	}
	else {
		alSourcei(p_Source, AL_SOURCE_SPATIALIZE_SOFT, AL_FALSE);
	}
}