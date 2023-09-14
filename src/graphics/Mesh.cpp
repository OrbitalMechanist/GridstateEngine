#include "graphics/Mesh.h"

Mesh::Mesh(GL_VertexArrayObject vaoToAssociate, GL_VertexBufferObject vboToAssociate,
	GL_ElementBufferObject eboToAssociate, GLuint numIndices) {
	vao = vaoToAssociate;
	vbo = vboToAssociate;
	ebo = eboToAssociate;
	indexCount = numIndices;
}

GL_VertexArrayObject Mesh::getVAO() {
	return vao;
}

GL_VertexBufferObject Mesh::getVBO() {
	return vbo;
}

GL_ElementBufferObject Mesh::getEBO() {
	return ebo;
}

GLuint Mesh::getIndexCount() {
	return indexCount;
}

Mesh::Mesh() {
	vao = 0;
	vbo = 0;
	ebo = 0;
	indexCount = 0;
}
