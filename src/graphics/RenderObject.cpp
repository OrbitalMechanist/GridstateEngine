#include "../headers/graphics/RenderObject.h"

RenderObject::RenderObject() {
	RenderObject("", "", "");
}

RenderObject::RenderObject(std::string model, std::string texture, std::string shader, 
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool isVisble, bool castsShadow)
{
	modelName = model;
	textureName = texture;
	shaderName = shader;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	visible = isVisble;
	shadowing = castsShadow;
}

RenderObject::RenderObject(std::string model, std::string texture, std::string shader)
{
	RenderObject(model, texture, shader, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f }, true, true);
};
