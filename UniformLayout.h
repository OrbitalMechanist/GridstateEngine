#pragma once

#include "GLTypes.h"

//This corresponds to our standard shader uniforms.
struct UniformLayout {
	GL_Uniform mvp;
	GL_Uniform diffuseTex;
	GL_Uniform normMat;

	UniformLayout() {
		mvp = -1;
		diffuseTex = -1;
		normMat = -1;
	}
};