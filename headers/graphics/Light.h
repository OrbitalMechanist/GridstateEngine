#include <glm/glm.hpp>
#include <glad/glad.h>

struct Light {
	//0 for off, 1 for directional, 2 for point, 3 for spot
	GLuint type;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	GLfloat intensity;
	GLfloat angle;
	GLfloat distanceLimit;
	GLfloat attenuationMax;
};