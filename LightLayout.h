#include "GLTypes.h"

struct LightLayout {
	GL_Uniform type;
	GL_Uniform position;
	GL_Uniform direction;
	GL_Uniform color;
	GL_Uniform intensity;
	GL_Uniform angle;
	GL_Uniform distanceLimit;
	GL_Uniform attenuationMax;

	LightLayout() {
		type = -1;
		position = -1;
		direction = -1;
		color = -1;
		intensity = -1;
		angle = -1;
		distanceLimit = -1;
		attenuationMax = -1;
	}
};