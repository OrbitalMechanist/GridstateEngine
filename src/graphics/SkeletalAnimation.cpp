#include "graphics/SkeletalAnimation.h"

void SkeletalAnimation::calculateFinalKeyframes()
{
	size_t numKeyframes = perBoneKeyframes.size();

	std::pair<size_t, std::vector<glm::mat4>> pairTemplate 
		= std::make_pair<size_t, std::vector<glm::mat4>>(0, std::vector<glm::mat4>());

	finalKeyframes = perBoneKeyframes; //this gets copied (I think/hope)

	if (!numKeyframes) {
		return; //can't calculate anything if there are no keyframes.
		//However, the finalKeyframes vector should still be set up, so this isn't done earlier.
	}

	size_t numBones = perBoneKeyframes[0].second.size();

	for (size_t i = 0; i < numKeyframes; ++i) {
		finalKeyframes[i].first = perBoneKeyframes[i].first; //the frame index is the same

		std::vector<bool> boneTransformationChecklist(false, numBones);
		//As far as I can tell, the first bone retrieved from Assimp will be the overall root.
		propagateBoneTransforms(i, 0, boneTransformationChecklist);

		for (size_t j = 0; j < numBones; ++j) {
			if (!boneTransformationChecklist[j]) {
				//Sometimes there are other unparented sections (in my case the IK bones).
				propagateBoneTransforms(i, j, boneTransformationChecklist);
			}
		}
	}
}

void SkeletalAnimation::propagateBoneTransforms(size_t keyframeIndex,
	size_t boneIndex, std::vector<bool>& checklist)
{
	auto& bone = skeleton.getBoneByIndex(boneIndex);

	if (bone.parent != boneIndex) {
		finalKeyframes[keyframeIndex].second[boneIndex]
			= finalKeyframes[keyframeIndex].second[boneIndex] 
			* finalKeyframes[keyframeIndex].second[bone.parent];
	}
	else {
		finalKeyframes[keyframeIndex].second[boneIndex]	= finalKeyframes[keyframeIndex].second[boneIndex];
	}
	checklist[boneIndex] = true;

	for (auto& i : bone.children) {
		propagateBoneTransforms(keyframeIndex, i, checklist);
	}
}

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

	calculateFinalKeyframes();
}

std::map<std::string, SkeletalAnimation> SkeletalAnimation::loadAllFromFile(Skeleton& skeleton, std::string path)
{
	return std::map<std::string, SkeletalAnimation>();
}

