#pragma once
//#include "graphics/SkeletalAnimation.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Skeleton {
	struct Bone {
		std::string name;
		size_t index;
		size_t parent;
		Bone();
	};
private:
	std::vector<Bone> bones;
public:
	//Loads a skeleton from a mesh using Assimp. The file must have only one skeleton in it!
	Skeleton(std::string path);
	Skeleton(size_t boneCount);
	//This serves for setting up the skeleton from an outside loader, or creating one by hand (don't do it).
	void setBone(std::string name, size_t index, size_t parentIndex);
	Bone& getBoneByIndex(size_t index);
	Bone& getBoneParentByIndex(size_t childIndex);

	friend std::ostream& operator<<(std::ostream& Str, Skeleton const& v);
};