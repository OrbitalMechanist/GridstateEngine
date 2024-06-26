#pragma once
#include "Constants.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "GLTypes.h"
#include "Mesh.h"
#include "Vertex.h"
#include "UniformLayout.h"
#include "Light.h"
#include "ShadowMap.h"
#include "ShadowCubeMap.h"
#include "RenderObject.h"

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
#include <array>
#include <list>

class Renderer
{
private:
	struct Material {
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};
	std::list<RenderObject> renderQueue;

	glm::vec3 cameraPos;
	glm::vec3 cameraRot;

	GLFWwindow* window;
	GL_Context context;

	uint32_t windowXsize, windowYsize;

	std::map<std::string, Mesh> models;
	std::map<std::string, GL_Texture> textures;
	std::map<std::string, Material> materials;
	std::map<std::string, ShaderProgram> shaderPrograms;

	std::array<Light, NUM_LIGHTS> lights;
	std::array<ShadowMap, NUM_LIGHTS> shadowMaps;
	std::array<ShadowCubeMap, NUM_LIGHTS> shadowCubemaps;
	ShaderProgram shadowShader;
	ShaderProgram cubeShadowShader;

	const GLuint shadowMapXsize = 1024;
	const GLuint shadowMapYsize = 1024;

	glm::vec4 backgroundColor;

	GL_Shader loadShader(const std::string& path, GLenum shaderStage);

	void createCubeModel();

	ShadowMap createShadowMap();

	ShadowCubeMap createShadowCubeMap();

	void setupShadowRender();
	
	void setupVisibleRender();

	void prepareCubeShadowRenderForLight(GLuint lightIndex);

	void prepareFlatShadowRenderForLight(GLuint lightIndex);

	void castShadow(GLuint lightIndex, const std::string& modelName, const glm::vec3& pos,
		const glm::vec3& rot, const glm::vec3& scale);

	void shadowRenderPass(bool clearBuffer);

	void mainRenderPass(bool clearBuffer);
public:
	Renderer(GLFWwindow* creatorWindow, uint32_t windowWidth, uint32_t windowHeight);

	void drawByNames(const std::string& modelName, const std::string& textureName, const std::string& shaderName, const 
		std::string& materialName, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

	bool loadTexture(const std::string& path, const std::string& resultName);

	bool loadModel(const std::string& path, const std::string& resultName);

	void createMaterial(const std::string& materialName, glm::vec3 diffuse, glm::vec3 specular, float shininess );

	bool loadShaderProgram(const std::string& vertPath, const std::string& geometryPath,
		const std::string& fragPath, const std::string& resultName);

	void setBackgroundColor(const glm::vec4& color);

	void clearFrame();

	void prepareForOperation();

	void updateWindowSize(GLFWwindow* wnd, int newX, int newY);

	void setCameraRotation(const glm::vec3& rot);

	void setCameraPosition(const glm::vec3& pos);

	glm::vec3 getCameraPosition();

	glm::vec3 getCameraRotation();

	//TODO: Somehow make sure removing models *actually* clears the memory.
	//Theoretically, the way it's done now should work though. As far as I can tell, anyways.

	bool removeTextureByName(const std::string& name);
	bool removeModelByName(const std::string& name);
	bool removeShaderProgramByName(const std::string& name);

	/// <summary>
	/// Sets the state of a light uniform and its associated struct.
	/// </summary>
	/// <param name="usableShaderName">The assigned string name of a shader program that contains a light array,
	///  previously loaded with loadShaderProgram(). Required because a shader that uses 
	///  these specific uniforms needs to be bound to set them. </param>
	/// <param name="lightIndex">Index of the light, [0:NUM_LIGHTS-1). There is a total of NUM_LIGHTS available. </param>
	/// <param name="type">What kind of light it is, and whether it's even on or not.
	/// 0 for off, 1 for directional, 2 for point, 3 for spot.</param>
	/// <param name="pos">Location of the light in world space, if applicable.</param>
	/// <param name="dir">Forward vector along which the light will shine,
	///  unless it's a point light, which is omnidirectional.</param>
	/// <param name="color">Color of the light, will also affect brightness 
	///  if components are set to values below, or above, 1.</param>
	/// <param name="intensity">Relative brightness of the light. Not measured in any particular units.</param>
	/// <param name="angle">Angle of arc for a spot light. Not applicable to other types.</param>
	/// <param name="distanceLimit">Distance beyond which the effects of a positional light will not be calculated.
	///  Set to negative to disable range cutoff. </param>
	/// <param name="attenuationMax">Distance at which the light's intensity will reach zero with linear faloff.
	///  Set to negative to disable attenuation.</param>
	/// <returns>bool, true if the light struct within the Renderer was updated and a write was attempted into the
	/// relevant uniform.</returns>
	bool setLightState(const std::string& usableShaderName, size_t lightIndex, GLuint type,
		const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& color, GLfloat intensity,
		GLfloat angle, GLfloat distanceLimit, GLfloat attenuationMax);

	/// <summary>
	/// Ambient light is a flat color value added to all lighting calculations. Useful for atmospheric stuff.
	/// </summary>
	/// <param name="usableShaderName">The assigned string name of a shader program that contains an ambient light,
	///  previously loaded with loadShaderProgram(). Required because a shader that uses 
	///  these specific uniforms needs to be bound to set them.</param>
	/// <param name="ambient">Color to add when calculating the final result.</param>
	void setAmbientLight(const std::string& usableShaderName, const glm::vec3& ambient);

	/// <summary>
	/// Adds a RenderObject to the render queue. It will be drawn at some point
	/// in accordance with its member parameter variables, but the order is not guaranteed.
	/// The methods by which it will be drawn are also not guaranteed.
	/// </summary>
	/// <param name="ro">RenderObject to add.</param>
	void addRenderObject(RenderObject ro);

	/// <summary>
	/// Renders all objects in the render queue and wipes it clean.
	/// </summary>
	/// <param name="clearBuffer">Whether to clear the frame buffer. Should probably be true
	///  but I'm leaving it optional just in case.</param>
	void renderFromQueue(bool clearBuffer);
};

