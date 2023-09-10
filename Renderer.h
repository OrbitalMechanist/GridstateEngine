#pragma once

#include "GLTypes.h"
#include "Mesh.h"
#include "Vertex.h"
#include "UniformLayout.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <istream>

class Renderer
{
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraRot;

	GLFWwindow* window;
	GL_Context context;

	uint32_t windowXsize, windowYsize;

	std::map<std::string, Mesh> models;
	std::map<std::string, GL_Texture> textures;
	std::map<std::string, GL_ShaderProgram> shaderPrograms;

	UniformLayout uniforms;

	GL_Shader loadShader(const std::string& path, GLenum shaderStage);

	void createCubeModel();
public:
	Renderer(GLFWwindow* creatorWindow, uint32_t windowWidth, uint32_t windowHeight);

	void drawByNames(const std::string& modelName, const std::string& textureName, const std::string& shaderName, 
		glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	bool loadTexture(const std::string& path, const std::string& resultName);

	bool loadModel(const std::string& path, const std::string& resultName);

	bool loadShaderProgram(const std::string& vertPath, const std::string& fragPath, const std::string& resultName);

	void setBackgroundColor(glm::vec4 color);

	void clearFrame();

	void updateWindowSize(int newX, int newY);

	void setCameraRotation(glm::vec3 rot);

	void setCameraPosition(glm::vec3 pos);

	glm::vec3 getCameraPosition();

	glm::vec3 getCameraRotation();

	//TODO: Somehow make sure removing models *actually* clears the memory.
	//Theoretically, the way it's done now should work though. As far as I can tell, anyways.

	bool removeTextureByName(const std::string& name);
	bool removeModelByName(const std::string& name);
	bool removeShaderProgramByName(const std::string& name);
};

