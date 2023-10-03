#pragma once
#include "graphics/GLTypes.h"
#include "glm/glm.hpp"

class ShadowMap
{
private:
	GL_FrameBufferObject fbo;
	GL_Texture map;
	glm::mat4 lightSpaceMatrix;
public:
	//Do not use
	ShadowMap();

	ShadowMap(GL_FrameBufferObject framebuffer, GL_Texture shadowMap);

	GL_FrameBufferObject getFBO();

	GL_Texture getMap();

	void updateLightSpaceMatrix(glm::mat4 newLSM);

	glm::mat4 getLightSpaceMatrix();
};

