#pragma once

#include "GLTypes.h"

//This corresponds to our standard shader uniforms.
struct UniformLayout {
	GL_Uniform mvp;
	GL_Uniform diffuseTex;
	GL_Uniform normMat;
	GL_Uniform modelMat;
	GL_Uniform lights;
	GL_Uniform ambientLight;

	UniformLayout() {
		mvp = -1;
		diffuseTex = -1;
		normMat = -1;
		modelMat = -1;
		lights = -1;
		ambientLight = -1;
	}
};