#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>

#include "Skeleton.h"
#include <map>

class SkeletalAnimation
{
private:
	Skeleton& skeleton;
	std::string name;

	//Length of the animation in frames; note that there are probably less keyframes than this.
	size_t frameLength;
	//Duration of the animation in seconds. Framerate may differ.
	float duration;
	//Number of keyframes in this animation.
	size_t keyframeCount;

	//Pre-calculated final transforms for each bone for each keyframe.
	//Cheaper than calculating the cascading effects of bones every single time.
	std::vector<std::pair<double, std::vector<glm::mat4>>> finalKeyframes;
	//Per-bone transforms for each keyframe. May sometimes be necessary, especially for any
	//procedural animations like IK and whatnot.
	std::vector<std::pair<double, std::vector<glm::mat4>>> perBoneKeyframes;

	//Overwrites the finalKeyframes vector with final frames based on the current contents of perBoneKeyframes.
	void calculateFinalKeyframes();

	//Recursively updates final bone transforms and sets the Checklist value to true for bone of that index.
	//Size of checklist should be equal to the number of bones in the skeleton.
	//Starts with bone at given index then calls itself on its children.
	void propagateBoneTransforms(size_t keyframeIndex, size_t boneIndex, std::vector<bool>& checklist);
public:
	//Length of the animation in frames; note that there are probably less keyframes than this.
	size_t getFrameLength();
	//Duration of the animation in seconds. Framerate may differ.
	float getDuration();
	//Number of keyframes in this animation.
	size_t getKeyframeCount();

	SkeletalAnimation(Skeleton& skel, size_t numFrames, float timeDuration,
		std::vector<std::pair<double, std::vector<glm::mat4>>> keyframes);

	//Returns a map containing all animations found in file at path,
	//assuming the given skeleton matches what's in the file.
	static std::map<std::string, SkeletalAnimation> loadAllFromFile(Skeleton& skeleton, std::string path);
};

