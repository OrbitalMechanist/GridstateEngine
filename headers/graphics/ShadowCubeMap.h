#pragma once

#include "GLTypes.h"
#include "glm/glm.hpp"

#include <array>

//Shadow cubemaps and regular, flat shadow maps are in no way interchangeable,
//so they are not linked via inheritance.
//This *could* be changed if we end up moving away from the Renderer god-class in favour
//of assembling it out of smaller subsystems, in which case a ShadowMap baseclass could take
//all the properties of a light and then do rendering in different ways based on whether it's
//actually a ShadowFlatMap or a ShadowCubeMap. For now, however, the Renderer knows very
//explicitly whether it's dealing with a texture or cubemap right now and uses both kinds of shadowmaps
//mainly as datastores.
class ShadowCubeMap
{
private:
	GL_FrameBufferObject fbo;
	GL_Cubemap cubemap;
	std::array<glm::mat4, 6>  cubemapFaceShadowMatrices;
public:
	//Do not use unless you're an std container
	ShadowCubeMap() = default;

	ShadowCubeMap(GL_FrameBufferObject framebuffer, GL_Cubemap shadowCubemap) : fbo(framebuffer),
		cubemap(shadowCubemap), cubemapFaceShadowMatrices() {};

	GL_FrameBufferObject getFBO();

	GL_Cubemap getCubemap();

	void updateMatrices(glm::mat4 face0, glm::mat4 face1, glm::mat4 face2,
		glm::mat4 face3, glm::mat4 face4, glm::mat4 face5);

	std::array<glm::mat4, 6> getFaceShadowMatrices();
};

