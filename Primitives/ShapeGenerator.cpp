#include <GL/glew.h>
#include "ShapeGenerator.h"
#include "Vertex.h"
#define ARR_SIZE(arr) sizeof(arr) / sizeof(arr[0])

using glm::vec3;

// size = size of the source and destination in bytes
void memcpy(void *dest, void *source, size_t size) {

	// copy byte by byte
	for (size_t i = 0; i < size; ++i) 
		*((char*)dest + i) = *((char*)source + i);
}

ShapeData* ShapeGenerator::makeTriangle()
{	
	ShapeData *triangle = new ShapeData();

	// set the vertices of the triangle and also their colors
	Position positions[] =
	{
		vec3(0.0f, 1.0f, 0.5f),
		vec3(-1.0f, -1.0f, 0.5f),
		vec3(-1.0f, 1.0f, 0.5f)
	};

	Color colors[] =
	{
		vec3(0.0f, 0.0f, 1.0f),
		vec3(1.0f, 0.0f, 1.0f),
		vec3(1.0f, 0.4f, 0.9f)
	};

	GLubyte indices[] = { 0,1,2 };

	// copy the position and color values
	triangle->numVertices = ARR_SIZE(positions);

	triangle->positions = new Position[triangle->numVertices];
	memcpy(triangle->positions, positions, sizeof(positions));

	triangle->colors = new Color[triangle->numVertices];
	memcpy(triangle->colors, colors, sizeof(colors));
	
	// copy the indices into triangle as well
	triangle->numIndices = ARR_SIZE(indices);
	triangle->indices = new GLubyte[triangle->numIndices];
	memcpy(triangle->indices, indices, sizeof(indices));

	return triangle;
}

ShapeData* ShapeGenerator::makeCube() {

	ShapeData* cube = new ShapeData();

	Position positions[] = {
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(-1.0f, 1.0f, -1.0f),

		vec3(-1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),

		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, -1.0f),

		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, 1.0f),

		vec3(1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f),

		vec3(1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, -1.0f),
		vec3(-1.0f, -1.0f, 1.0f),
		vec3(1.0f, -1.0f, 1.0f),
	};

	Color colors[] = {
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),

		vec3(1.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.5f, 0.2f),
		vec3(0.8f, 0.6f, 0.4f),
		vec3(0.3f, 1.0f, 0.5f),

		vec3(0.2f, 0.5f, 0.2f),
		vec3(0.9f, 0.3f, 0.7f),
		vec3(0.3f, 0.7f, 0.5f),
		vec3(0.5f, 0.7f, 0.5f),

		vec3(0.7f, 0.8f, 0.2f),
		vec3(0.5f, 0.7f, 0.3f),
		vec3(0.4f, 0.7f, 0.7f),
		vec3(0.2f, 0.5f, 1.0f),

		vec3(0.6f, 1.0f, 0.7f),
		vec3(0.6f, 0.4f, 0.8f),
		vec3(0.2f, 0.8f, 0.7f),
		vec3(0.2f, 0.7f, 1.0f),

		vec3(0.8f, 0.3f, 0.7f),
		vec3(0.8f, 0.9f, 0.5f),
		vec3(0.5f, 0.8f, 0.5f),
		vec3(0.9f, 1.0f, 0.2f),
	};

	cube->numVertices = ARR_SIZE(positions);
	cube->positions = new Position[cube->numVertices];
	memcpy(cube->positions, positions, sizeof(positions));

	cube->colors = new Color[cube->numVertices];
	memcpy(cube->colors, colors, sizeof(colors));

	GLubyte indices[] = {

		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 22, 21, 20, 23, 22
	};

	cube->numIndices = ARR_SIZE(indices);
	cube->indices = new GLubyte[cube->numIndices];
	memcpy(cube->indices, indices, sizeof(indices));

	return cube;
}