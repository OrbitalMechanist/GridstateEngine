/*
* Credits:
* https://www.youtube.com/watch?v=kWQM1iQ1W0E
* For providing a basic OpenAL tutorial on setting up and playing sounds
*/
#pragma once
#include <OpenAL\al.h>
#include <vector>

/// Gets and holds raw sound files
class SoundBuffer
{
public:
	static SoundBuffer* get();

	ALuint addSoundEffect(const char* filename);
	bool removeSoundEffect(const ALuint& buffer);

private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> p_SoundEffectBuffers;
};

