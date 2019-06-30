#pragma once

#include <GL/glew.h>
#include "Vertex.h"
#include <iostream>

struct Vertex;
struct Color;
class ShapeGenerator;

class ShapeData {

	Vertex* vertices;
	Color* colors;
	GLuint numVertices;  // number of vertices = number of colors

	GLushort* indices;
	GLuint numIndices;  // only 256 unique vertices

public:

	ShapeData() : vertices(nullptr), colors(nullptr),
		numVertices(0), indices(nullptr),
		numIndices(0) {}

	GLsizeiptr vertexBufferSize() const { return numVertices * sizeof(Vertex); }

	GLsizeiptr colorBufferSize() const { return numVertices * sizeof(Color); }

	GLsizeiptr indexBufferSize() const { return numIndices * sizeof(GLushort); }

	Vertex* vertexData() const { return vertices; }

	Color* colorData() const { return colors; }

	GLushort* indexData() const { return indices; }

	GLuint getNumIndices() const { return numIndices; }

	void transformNormals(const glm::mat4& rotationMatrix);
	
	~ShapeData() {
		delete[] vertices;
		delete[] indices;
		delete[] colors;
	}

	friend class ShapeGenerator;
};
