#include "meshSurface.h"

#include <iostream>

MeshSurface::MeshSurface(unsigned int x, unsigned int y, unsigned int vSize, float w, float h) {
	if (vSize < 2) {
		std::cout << "ERROR : Could not create MeshSurface Object: vSize must be at leat 2.\n";
		exit(1);
	}
	
	vertDimensions[0] = x;
	vertDimensions[1] = y;
	vertSize = vSize;
	// Create vertex array
	int va_size = vSize * x * y;
	vertices = new float[va_size];
	vertexCount = x * y;

	for (int yi = 0; yi < y; yi++) {
		for (int xi = 0; xi < x; xi++) {
			float x_pos = (xi / float(x - 1)) * w - 1.0f;
			float y_pos = (yi / float(y - 1)) * h - 1.0f;
			
			int vi = vSize * (xi + (yi * x));
			vertices[vi] = x_pos;
			vertices[vi + 1] = y_pos;
			//std::cout << vi << ": (" << x_pos << ", " << y_pos;
			for (int di = 2; di < vSize; di++) {
				vertices[vi + di] = 0.0f;
				//std::cout << ", 0";
			}
			//std::cout << ")\n";
		}
	}

	//vertices[(4 * 300) + 3] = 1.0;

	glBindVertexArray(getVertexArray());
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vSize * sizeof(float), vertices, GL_DYNAMIC_DRAW);
	/*
	for (int i = 0; i < va_size; i++) {
		
		if (i % vSize == 0) {
			std::cout << "\n";
		}
		std::cout << vertices[i] << ", ";
	}
	*/

	// Create Index array
	int ia_size = 6 * (x - 1) * (y - 1); 
	indices = new unsigned int[ia_size];
	indexCount = ia_size;

	/*
		vi = vSize * (xi + (yi * x))
		vert[xi][yi] = vert[vi]
		tris = {(xi, yi), (xi + 1, yi), (xi, yi + 1)}, {(xi + 1, yi), (xi + 1, yi + 1), (xi, yi + 1)}
	*/

	for (int yi = 0; yi < y - 1; yi++) {
		for (int xi = 0; xi < x - 1; xi++) {
			int i = 6 * (xi + yi * (x - 1));
			//tri 1
			indices[i] = (xi + (yi * x));
			indices[i + 1] = (xi + 1 + (yi * x));
			indices[i + 2] = (xi + ((yi + 1) * x));
			//tri 2
			indices[i + 3] = (xi + 1 + (yi * x));
			indices[i + 4] = (xi + 1 + ((yi + 1) * x));
			indices[i + 5] = (xi + ((yi + 1) * x));
		}
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), indices, GL_DYNAMIC_DRAW);

	/*
	for (int i = 0; i < ia_size; i++) {
		if (i % 3 == 0) {
			std::cout << "\n";
		}
		std::cout << indices[i] << ", ";
	}
	*/
}