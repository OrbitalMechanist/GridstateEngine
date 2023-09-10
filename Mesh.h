#pragma once
#include "GLTypes.h"

class Mesh
{
private:
	GL_VertexArrayObject vao;
	GL_VertexBufferObject vbo;
	GL_ElementBufferObject ebo;
	GLuint indexCount;

public:
	//Do NOT use this. You can't actually change the placeholder values.
	Mesh();

	Mesh(GL_VertexArrayObject vaoToAssociate, GL_VertexBufferObject vboToAssociate,
		GL_ElementBufferObject eboToAssociate, GLuint numIndices);

	GL_VertexArrayObject getVAO();
	GL_VertexBufferObject getVBO();
	GL_ElementBufferObject getEBO();
	GLuint getIndexCount();
};

