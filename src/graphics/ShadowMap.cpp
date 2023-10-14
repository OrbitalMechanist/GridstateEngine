#include "graphics/ShadowMap.h"

ShadowMap::ShadowMap() {
	map = 0;
	fbo = 0;
}

ShadowMap::ShadowMap(GL_FrameBufferObject framebuffer, GL_Texture shadowMap)
{
	fbo = framebuffer;
	map = shadowMap;
}

GL_FrameBufferObject ShadowMap::getFBO()
{
	return fbo;
}

GL_Texture ShadowMap::getMap()
{
	return map;
}

void ShadowMap::updateLightSpaceMatrix(glm::mat4 newLSM)
{
	lightSpaceMatrix = newLSM;
}

glm::mat4 ShadowMap::getLightSpaceMatrix()
{
	return lightSpaceMatrix;
}
