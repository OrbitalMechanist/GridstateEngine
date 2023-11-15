#pragma once
#include <glm/glm.hpp>
#include <string>
#include <iostream>

//This is NOT intended for long-term storage, only for render queue usage.
//The Universe will take orders from GameMaster and any other object lists such as
//physics, parse them into world coordinates, and give the Renderer a bunch of RenderObjects
//to deal with during that particular frame. Then the ROs will be deleted when the next frame
//comes around.
class RenderObject
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool visible;
	bool shadowing;
	std::string modelName;
	std::string textureName;
	std::string shaderName;
	std::string material;

	//Probably best not to use this.
	RenderObject();

	RenderObject(std::string model, std::string texture, std::string shader);

	RenderObject(std::string model, std::string texture, std::string shader,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		bool isVisble = true, bool castsShadow = true);

	RenderObject(std::string model, std::string texture, std::string material, std::string shader,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		bool isVisble = true, bool castsShadow = true);
};
