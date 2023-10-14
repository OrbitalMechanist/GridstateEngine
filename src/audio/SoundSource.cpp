#include "audio/SoundSource.h"
#include <iostream>

//Sets up a sound source to play audio
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

//Deletes sources when instance of sound source is destroyed
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

//Pause audio
void SoundSource::Pause()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING) {
		alSourcePause(p_Source);
	}
}

//Stop audio
void SoundSource::Stop()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING) {
		alSourceStop(p_Source);
	}
}

//Sets the pitch of the audio
void SoundSource::SetPitch(float pitch) {
	alSourcef(p_Source, AL_PITCH, pitch);
}

//Sets the gain of the audio
void SoundSource::SetGain(float gain) {
	alSourcef(p_Source, AL_GAIN, gain);
}

//Sets position of audio source
void SoundSource::SetPosition(glm::vec3 pos) {
	alSource3f(p_Source, AL_POSITION, pos.x, pos.y, pos.z);
}

//Sets velocity of audio source
void SoundSource::SetVelocity(glm::vec3 vel) {
	alSource3f(p_Source, AL_VELOCITY, vel.x, vel.y, vel.z);
}

//Sets to whether the audio source loops or not
void SoundSource::SetLooping(bool looping) {
	alSourcei(p_Source, AL_LOOPING, looping);
}

//Gets current pitch of audio
float SoundSource::GetPitch() {
	float f;
	alGetSourcef(p_Source, AL_PITCH, &f);
	return f;
}

//Gets current gain of audio
float SoundSource::GetGain() {
	float g;
	alGetSourcef(p_Source, AL_GAIN, &g);
	return g;
}

//Gets current position of audio
glm::vec3 SoundSource::GetPosition() {
	float x; float y; float z;
	alGetSource3f(p_Source, AL_POSITION, &x, &y, &z);
	return { x, y, z };
}

//Gets current velocity of audio
glm::vec3 SoundSource::GetVelocity() {
	float x; float y; float z;
	alGetSource3f(p_Source, AL_VELOCITY, &x, &y, &z);
	return { x, y, z };
}

//Sets whether audio source is positional audio or not
void SoundSource::SetPositionalAudio(bool positionalAudio) {
	if (positionalAudio) {
		alSourcei(p_Source, AL_SOURCE_SPATIALIZE_SOFT, AL_TRUE);
	}
	else {
		alSourcei(p_Source, AL_SOURCE_SPATIALIZE_SOFT, AL_FALSE);
	}
}