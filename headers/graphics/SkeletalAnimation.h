#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>

#include "Skeleton.h"

class SkeletalAnimation
{
private:
	const Skeleton& skeleton;
	std::string name;

	//Length of the animation in frames; note that there are probably less keyframes than this.
	size_t frameLength;
	//Duration of the animation in seconds. Framerate may differ.
	float duration;
	//Number of keyframes in this animation.
	size_t keyframeCount;

	//Pre-calculated final transforms for each bone for each keyframe.
	//Cheaper than calculating the cascading effects of bones every single time.
	std::vector<std::pair<size_t, std::vector<glm::mat4>>> finalKeyframes;
	//Per-bone transforms for each keyframe. May sometimes be necessary, especially for any
	//procedural animations like IK and whatnot.
	std::vector<std::pair<size_t, std::vector<glm::mat4>>> perBoneKeyframes;
public:
	//Length of the animation in frames; note that there are probably less keyframes than this.
	size_t getFrameLength();
	//Duration of the animation in seconds. Framerate may differ.
	float getDuration();
	//Number of keyframes in this animation.
	size_t getKeyframeCount();

	SkeletalAnimation(Skeleton& skel, size_t numFrames, float timeDuration,
		std::vector<std::pair<size_t, std::vector<glm::mat4>>> keyframes);

};

