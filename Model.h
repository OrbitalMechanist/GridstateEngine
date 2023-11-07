#pragma once
#include "Bone.h"
#include "graphics/Mesh.h"
#include "graphics/Vertex.h"
#include <map>
#include <string>
#include <vector>
#include <assimp/scene.h>

class Model
{
private:
	std::map < std::string, BoneInfo > m_BoneInfoMap;
	int m_BoneCounter = 0;

	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }

	void SetVertexBoneDataToDefault(Vertex& vertex);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector& vertices, aiMesh* mesh, const aiScene* scene);
};

