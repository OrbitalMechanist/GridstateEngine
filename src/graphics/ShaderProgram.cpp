#include "graphics/ShaderProgram.h"
#include <iostream>
ShaderProgram::ShaderProgram(GL_ShaderProgram assoc) {
	glReference = assoc;
	uniforms = UniformLayout();
}

ShaderProgram::ShaderProgram() {
	glReference = 0;
}

GL_ShaderProgram ShaderProgram::getGLReference() {
	return glReference;
}

void ShaderProgram::queryUniformLocations() {
	GL_Uniform tmp;
	tmp = glGetUniformLocation(glReference, "mvp");
	uniforms.mvp = tmp;
	tmp = glGetUniformLocation(glReference, "diffuseTex");
	uniforms.diffuseTex = tmp;
	tmp = glGetUniformLocation(glReference, "normMat");
	uniforms.normMat = tmp;
	tmp = glGetUniformLocation(glReference, "modelMat");
	uniforms.modelMat = tmp;
	attemptLightLoading();
	tmp = glGetUniformLocation(glReference, "ambientLight");
	uniforms.ambientLight = tmp;
	tmp = glGetUniformLocation(glReference, "viewPos");
	uniforms.viewPos = tmp;
	tmp = glGetUniformLocation(glReference, "material.diffuse");
	uniforms.material = tmp;
	//std::cout << "Diffuse: " << tmp << std::endl;
	/**tmp = glGetUniformLocation(glReference, "material.specular");
	uniforms.material = tmp;
	std::cout << "Specular: " << tmp << std::endl;
	tmp = glGetUniformLocation(glReference, "material.shininess");
	uniforms.material = tmp;
	std::cout << "Shininess: " << tmp << std::endl;*/
	tmp = glGetUniformLocation(glReference, "shadowMaps[0]");
	uniforms.shadowMapFirstElement = tmp;
	tmp = glGetUniformLocation(glReference, "shadowCubemaps[0]");
	uniforms.shadowCubemapFirstElement = tmp;
	tmp = glGetUniformLocation(glReference, "lightSpaceMatrices[0]");
	uniforms.lightSpaceMatrixFirstElement = tmp;
	tmp = glGetUniformLocation(glReference, "shadowFaces[0]");
	uniforms.cubeShadowFacesFirstElement = tmp;
	tmp = glGetUniformLocation(glReference, "cubeShadowLightPos");
	uniforms.cubeShadowLightPos = tmp;
	tmp = glGetUniformLocation(glReference, "cubeShadowDistanceLimit");
	uniforms.cubeShadowDistanceLimit = tmp;

	/*
	std::cout << "mvp: " << uniforms.mvp << " difTex: " << uniforms.diffuseTex
		<< " normMat: " << uniforms.normMat << " modMat: " << uniforms.modelMat
		<< " ambient: " << uniforms.ambientLight << " view: " << uniforms.viewPos
		<< " mat: " << uniforms.material << " shadows: " << uniforms.shadowMapFirstElement
		<< " cube: " << uniforms.shadowCubemapFirstElement << " lsm: " << uniforms.lightSpaceMatrixFirstElement
		<< " shadowFaces: " << uniforms.cubeShadowFacesFirstElement
		<< " cslp: " << uniforms.cubeShadowLightPos << " csdl: " << uniforms.cubeShadowDistanceLimit << std::endl;
	*/
}

const UniformLayout& ShaderProgram::referenceUniforms() {
	return uniforms;
}

bool ShaderProgram::attemptLightLoading() {
	//If lights[0].type is a valid uniform we assume it's a standard lights array.
	//Otherwise, we assume there are no lights at all.
	GL_Uniform test = glGetUniformLocation(glReference, "lights[0].type");
	if (test == -1) {
		return false;
	}
	for (size_t i = 0; i < NUM_LIGHTS; i++) {
		uniforms.lights[i].type = glGetUniformLocation(glReference, ("lights[" + std::to_string(i) + "].type").c_str());
		uniforms.lights[i].position = glGetUniformLocation(glReference,
			("lights[" + std::to_string(i) + "].position").c_str());
		uniforms.lights[i].direction = glGetUniformLocation(glReference,
			("lights[" + std::to_string(i) + "].direction").c_str());
		uniforms.lights[i].color = glGetUniformLocation(glReference, ("lights[" + std::to_string(i) + "].color").c_str());
		uniforms.lights[i].intensity = glGetUniformLocation(glReference,
			("lights[" + std::to_string(i) + "].intensity").c_str());
		uniforms.lights[i].angle = glGetUniformLocation(glReference, ("lights[" + std::to_string(i) + "].angle").c_str());
		uniforms.lights[i].distanceLimit = glGetUniformLocation(glReference,
			("lights[" + std::to_string(i) + "].distanceLimit").c_str());
		uniforms.lights[i].attenuationMax = glGetUniformLocation(glReference,
			("lights[" + std::to_string(i) + "].attenuationMax").c_str());
	}
}