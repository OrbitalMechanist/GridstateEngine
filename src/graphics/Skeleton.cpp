#include "graphics/Skeleton.h"

Skeleton::Skeleton(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_PopulateArmatureData);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Skeleton loading failure: " << import.GetErrorString() << std::endl;
		return;
	}

	if (scene->mMeshes == 0) {
		std::cerr << "Skeleton loading failure: scene has no meshes." << std::endl;
		return;
	}
	
	if (!scene->mMeshes[0]->HasBones()) {
		std::cerr << "Skeleton loading failure: mesh has no bones." << std::endl;
		return;
	}

	auto bonesArr = scene->mMeshes[0]->mBones;
	size_t boneCount = scene->mMeshes[0]->mNumBones;

	Bone og = Bone();
	bones = std::vector<Bone>(boneCount, og);

	//This loop fills the bone array with names and indices.
	for (int i = 0; i < boneCount; i++) {
		bones[i].index = i;
		bones[i].name = std::string(bonesArr[i]->mName.C_Str());
		bones[i].parent = bonesArr[i]->mArmature->mParent == nullptr ? i : 0;
	}

	//This sets the parent relations of bones.
	for (int i = 0; i < boneCount; i++) {
		auto potentialParent = bonesArr[i]->mArmature->FindNode(bones[i].name.c_str());;
		for (int j = 0; j < potentialParent->mNumChildren; j++) {
			auto kidName = potentialParent->mChildren[j]->mName.C_Str();
			for (int k = 0; k < boneCount; k++) {
				if (bones[k].name == kidName) {
					bones[k].parent = i;
					bones[i].children.push_back(k);
					break;
				}
			}
		}
	}

	//Generally speaking, this is somewhat scuffed and not exactly efficient at load time.
	//However, once I get it fully working, this will allow most animation math to happen
	//during initial loading, which isn't nearly as demanding in terms of speed as runtime calculations.
	//At least I hope so.
}

Skeleton::Skeleton(size_t boneCount)
{
	Bone og = Bone();
	bones = std::vector<Bone>( boneCount, og );
}

void Skeleton::setBone(std::string name, size_t index, size_t parentIndex)
{
	bones[index].index = index; //derp
	bones[index].name = name;
	bones[index].parent = parentIndex;
	bones[parentIndex].children.push_back(index);
}

const Bone& Skeleton::getBoneByIndex(size_t index)
{
	return bones[index];
}

const Bone& Skeleton::getBoneParentByIndex(size_t childIndex)
{
	return bones[bones[childIndex].parent];
}

std::ostream& operator<<(std::ostream& Str, Skeleton const& v)
{
	Str << std::string("SKELETON with ") << v.bones.size() << std::string(" bones:");
	for (auto& i : v.bones) {
		Str << std::string("\nBone: Index ") << i.index << std::string(" named \"")
			<< i.name << std::string("\", children:");
		
		if (i.children.size() == 0) {
			Str << std::string("NONE");
		}
		else {
			for (auto j : i.children) {
				Str << std::string(" ") << j;
			}
		}

		Str << std::string(", parent index ") << i.parent;
		if (i.parent == i.index) {
			Str << std::string(" (unparented, possible root)");
		}
	}

	return Str;
}
