#include "../headers/graphics/ShadowCubeMap.h"

GL_FrameBufferObject ShadowCubeMap::getFBO() {
	return fbo;
}

GL_Cubemap ShadowCubeMap::getCubemap() {
	return cubemap;
}

void ShadowCubeMap::updateMatrices(glm::mat4 face0, glm::mat4 face1, glm::mat4 face2,
	glm::mat4 face3, glm::mat4 face4, glm::mat4 face5)
{
	cubemapFaceShadowMatrices[0] = face0;
	cubemapFaceShadowMatrices[1] = face1;
	cubemapFaceShadowMatrices[2] = face2;
	cubemapFaceShadowMatrices[3] = face3;
	cubemapFaceShadowMatrices[4] = face4;
	cubemapFaceShadowMatrices[5] = face5;
}

std::array<glm::mat4, 6> ShadowCubeMap::getFaceShadowMatrices()
{
	return cubemapFaceShadowMatrices;
}

