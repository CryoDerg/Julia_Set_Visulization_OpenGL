#include "mesh.h"
#include <iostream>

Mesh::Mesh() {
	// Create Vertex Array Object to store vertex attributes and their cooresponding buffer
	glGenVertexArrays(1, &vertex_array); // Create empty Vertex Array Object
	glBindVertexArray(vertex_array); // Bind the Vertex Array Object to store any future formatting of Vertex Data

	// Define a buffer to send data to the GPU, bind it, and add verts to it
   glGenBuffers(1, &vertex_buffer); // Create Buffer Object
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); // Bind the Object to the GL_ARRAY_BUFFER target
   
	// Create an Element Buffer Object that draw primitives using indicies of verticies
	glGenBuffers(1, &element_buffer); // Create Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer); // Bind the object to the GL_ELEMENT_ARRAY_BUFFER target
}

void Mesh::addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset) {
	glBindVertexArray(vertex_array);
	// Format Vertex Data
	glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
	glEnableVertexAttribArray(index); // Enable the vertex attribute
}

void Mesh::setVertices(float* vertexList, unsigned int count, GLenum usage) {
	vertices = vertexList;
	vertexCount = count;

	glBindVertexArray(vertex_array);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, usage); // Add data to the buffer bound to GL_ARRAY_BUFFER
	/*
		GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		GL_STATIC_DRAW: the data is set only once and used many times.
		GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	*/
}

void Mesh::setIndices(unsigned int* indexList, unsigned int count, GLenum usage) {
	indices = indexList;
	indexCount = count;

	glBindVertexArray(vertex_array);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(int), indices, usage); // Add the index data to the buffer
}

void Mesh::updateVertices(GLintptr offset, GLintptr size) {
	//glBindVertexArray(vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices + (offset / sizeof(float)));
}

void Mesh::updateIndices(GLintptr offset, GLintptr size) {
	//glBindVertexArray(vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, indices);
}

void Mesh::draw() {
	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // Draw the rectangle as described by the verticies and indicies
}

unsigned int Mesh::getVertexArray() {
	return vertex_array;
}
