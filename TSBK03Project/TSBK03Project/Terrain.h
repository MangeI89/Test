#pragma once
#include <GL\glew.h>
#include <vector>
#include "Mesh.h"

struct Terrain {

public:
	Terrain(Vertex *vertexArray, GLuint *indexArray, GLuint numIndices, GLuint numVertices) {
		this->vertexArray = vertexArray;
		this->indexArray = indexArray;
		this->numIndices = numIndices;
		this->numVertices = numVertices;
	}

	Vertex *GetVertexArray() { return vertexArray; }

	unsigned int *GetIndexArray() { return indexArray; }

	unsigned int GetNumIndices() { return numIndices; }

	unsigned int GetNumVertices() {
		return numVertices;
	}

private:
	Vertex *vertexArray;
	unsigned int *indexArray;
	unsigned int numIndices;
	unsigned int numVertices;
};