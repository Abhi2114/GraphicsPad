#pragma once

#include <GL/glew.h>
#include "Vertex.h"
#include <iostream>

struct Position;
struct Color;
class ShapeGenerator;

class ShapeData {

	Position* positions;
	Color* colors;
	GLuint numVertices;  // number of vertices = number of colors

	GLushort* indices;
	GLuint numIndices;  // only 256 unique vertices

public:

	ShapeData() : positions(nullptr), colors(nullptr),
		numVertices(0), indices(nullptr),
		numIndices(0) {}

	GLsizeiptr positionBufferSize() const { return numVertices * sizeof(Position); }

	GLsizeiptr colorBufferSize() const { return numVertices * sizeof(Color); }

	GLsizeiptr indexBufferSize() const { return numIndices * sizeof(GLushort); }

	Position* positionData() const { return positions; }

	Color* colorData() const { return colors; }

	GLushort* indexData() const { return indices; }

	GLuint getNumIndices() const { return numIndices; }

	~ShapeData() {
		delete[] positions;
		delete[] indices;
		delete[] colors;
	}

	friend class ShapeGenerator;
};
