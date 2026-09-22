#ifndef MESHSURFACE_H
#define MESHSURFACE_H

#include "mesh.h"

class MeshSurface : public Mesh {
public:
	unsigned int vertDimensions[2];
	unsigned int vertSize;

	
	MeshSurface(unsigned int x, unsigned int y, unsigned int vSize, float w = 2.0f, float h = 2.0f);

};

#endif