#include "graphics/SkeletalAnimation.h"

size_t SkeletalAnimation::getFrameLength()
{
	return frameLength;
}

float SkeletalAnimation::getDuration()
{
	return duration;
}

size_t SkeletalAnimation::getKeyframeCount()
{
	return keyframeCount;
}

SkeletalAnimation::SkeletalAnimation(Skeleton& skel, size_t numFrames, float timeDuration,
	std::vector<std::pair<size_t, std::vector<glm::mat4>>> keyframes) 
	: skeleton(skel), frameLength(numFrames), duration(timeDuration)
{
	perBoneKeyframes = keyframes;
	keyframeCount = perBoneKeyframes.size();

	//TODO: Calculate final keyframes here. Probably with a helper function.
}
