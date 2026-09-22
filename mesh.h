#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>

class Mesh {
private:
	unsigned int vertex_array;
	unsigned int vertex_buffer;
	unsigned int element_buffer;
public:
	float* vertices;
	unsigned int vertexCount;
	unsigned int* indices;
	unsigned int indexCount;

	Mesh();

	void addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset);
	void setVertices(float* vertexList, unsigned int count, GLenum usage);
	void setIndices(unsigned int* indexList, unsigned int count, GLenum usage);

	void updateVertices(GLintptr offset, GLintptr size);
	void updateIndices(GLintptr offset, GLintptr size);

	void draw();

	unsigned int getVertexArray();
};

#endif