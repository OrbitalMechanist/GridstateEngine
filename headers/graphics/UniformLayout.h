#pragma once

#include "GLTypes.h"
#include "LightLayout.h"
#include "Constants.h"

#include <array>

//This corresponds to our standard shader uniforms.
struct UniformLayout {
	GL_Uniform mvp;
	GL_Uniform material;
	GL_Uniform diffuseTex;
	GL_Uniform normMat;
	GL_Uniform modelMat;
	std::array<LightLayout, NUM_LIGHTS> lights;
	GL_Uniform ambientLight;
	GL_Uniform shadowMapFirstElement;
	GL_Uniform shadowCubemapFirstElement;
	GL_Uniform lightSpaceMatrixFirstElement;
	GL_Uniform cubeShadowFacesFirstElement;
	GL_Uniform cubeShadowLightPos;
	GL_Uniform cubeShadowDistanceLimit;

	UniformLayout() {
		mvp = -1;
		material = -1;
		diffuseTex = -1;
		normMat = -1;
		modelMat = -1;
		ambientLight = -1;
		shadowMapFirstElement = -1;
		shadowCubemapFirstElement = -1;
		lightSpaceMatrixFirstElement = -1;
		cubeShadowFacesFirstElement = -1;
		cubeShadowLightPos = -1;
		cubeShadowDistanceLimit = -1;
	}
};