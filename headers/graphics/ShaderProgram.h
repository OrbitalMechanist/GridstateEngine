#pragma once
#include "UniformLayout.h"
#include <string>

class ShaderProgram
{
private:
	GL_ShaderProgram glReference;
	UniformLayout uniforms;

	bool attemptLightLoading();
public:
	//Do not use this. It exists only to enable std::map operations.
	ShaderProgram();

	ShaderProgram(GL_ShaderProgram assoc);

	GL_ShaderProgram getGLReference();

	void queryUniformLocations();

	const UniformLayout& referenceUniforms();
};

