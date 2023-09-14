#pragma once

#include "GLTypes.h"
#include "LightLayout.h"
#include "Constants.h"

#include <array>

//This corresponds to our standard shader uniforms.
struct UniformLayout {
	GL_Uniform mvp;
	GL_Uniform diffuseTex;
	GL_Uniform normMat;
	GL_Uniform modelMat;
	std::array<LightLayout, NUM_LIGHTS> lights;
	GL_Uniform ambientLight;

	UniformLayout() {
		mvp = -1;
		diffuseTex = -1;
		normMat = -1;
		modelMat = -1;
		ambientLight = -1;
	}
};