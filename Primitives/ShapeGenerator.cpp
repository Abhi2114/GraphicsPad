#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include "ShapeGenerator.h"
#include "Vertex.h"
#define ARR_SIZE(arr) sizeof(arr) / sizeof(arr[0])

using glm::vec3;

glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

// cleanup the temporary pointers used in various functions
void ShapeGenerator::cleanUpTemp(ShapeData *tempData) {
	// set all pointers to null before deleting
	tempData->vertices = nullptr;
	tempData->colors = nullptr;
	tempData->indices = nullptr;

	delete tempData;
}

// size = size of the source and destination in bytes
void memcpy(void* dest, void* source, size_t size) {

	// copy byte by byte
	for (size_t i = 0; i < size; ++i)
		* ((char*)dest + i) = *((char*)source + i);
}

ShapeData * ShapeGenerator::makeTriangle()
{
	ShapeData* triangle = new ShapeData();

	// set the vertices of the triangle and also their colors
	Vertex vertices[] =
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

	GLushort indices[] = { 0,1,2 };

	// copy the position and color values
	triangle->numVertices = ARR_SIZE(vertices);

	triangle->vertices = new Vertex[triangle->numVertices];
	memcpy(triangle->vertices, vertices, sizeof(vertices));

	triangle->colors = new Color[triangle->numVertices];
	memcpy(triangle->colors, colors, sizeof(colors));

	// copy the indices into triangle as well
	triangle->numIndices = ARR_SIZE(indices);
	triangle->indices = new GLushort[triangle->numIndices];
	memcpy(triangle->indices, indices, sizeof(indices));

	return triangle;
}

ShapeData* ShapeGenerator::makeCube() {

	ShapeData* cube = new ShapeData();

	Vertex vertices[] = {
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

	cube->numVertices = ARR_SIZE(vertices);
	cube->vertices = new Vertex[cube->numVertices];
	memcpy(cube->vertices, vertices, sizeof(vertices));

	cube->colors = new Color[cube->numVertices];
	memcpy(cube->colors, colors, sizeof(colors));

	GLushort indices[] = {

		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 22, 21, 20, 23, 22
	};

	cube->numIndices = ARR_SIZE(indices);
	cube->indices = new GLushort[cube->numIndices];
	memcpy(cube->indices, indices, sizeof(indices));

	return cube;
}

ShapeData * ShapeGenerator::makeArrow()
{
	ShapeData* arrow = new ShapeData();

	vec3 stackVerts[] =
	{
		// Top side of arrow head
		vec3(+0.00f, +0.25f, -0.25f),         // 0
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec3(+0.50f, +0.25f, -0.25f),         // 1
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec3(+0.00f, +0.25f, -1.00f),         // 2
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec3(-0.50f, +0.25f, -0.25f),         // 3
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		// Bottom side of arrow head
		vec3(+0.00f, -0.25f, -0.25f),         // 4
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec3(+0.50f, -0.25f, -0.25f),         // 5
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec3(+0.00f, -0.25f, -1.00f),         // 6
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec3(-0.50f, -0.25f, -0.25f),         // 7
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		// Right side of arrow tip
		vec3(+0.50f, +0.25f, -0.25f),         // 8
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec3(+0.00f, +0.25f, -1.00f),         // 9
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec3(+0.00f, -0.25f, -1.00f),         // 10
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec3(+0.50f, -0.25f, -0.25f),         // 11
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		// Left side of arrow tip
		vec3(+0.00f, +0.25f, -1.00f),         // 12
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec3(-0.50f, +0.25f, -0.25f),         // 13
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec3(+0.00f, -0.25f, -1.00f),         // 14
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec3(-0.50f, -0.25f, -0.25f),         // 15
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		// Back side of arrow tip
		vec3(-0.50f, +0.25f, -0.25f),         // 16
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec3(+0.50f, +0.25f, -0.25f),         // 17
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec3(-0.50f, -0.25f, -0.25f),         // 18
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec3(+0.50f, -0.25f, -0.25f),         // 19
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		// Top side of back of arrow
		vec3(+0.25f, +0.25f, -0.25f),         // 20
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec3(+0.25f, +0.25f, +1.00f),         // 21
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec3(-0.25f, +0.25f, +1.00f),         // 22
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		vec3(-0.25f, +0.25f, -0.25f),         // 23
		vec3(+1.00f, +0.00f, +0.00f),		  // Color
		vec3(+0.00f, +1.00f, +0.00f),         // Normal
		// Bottom side of back of arrow
		vec3(+0.25f, -0.25f, -0.25f),         // 24
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec3(+0.25f, -0.25f, +1.00f),         // 25
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec3(-0.25f, -0.25f, +1.00f),         // 26
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		vec3(-0.25f, -0.25f, -0.25f),         // 27
		vec3(+0.00f, +0.00f, +1.00f),		  // Color
		vec3(+0.00f, -1.00f, +0.00f),         // Normal
		// Right side of back of arrow
		vec3(+0.25f, +0.25f, -0.25f),         // 28
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec3(+0.25f, -0.25f, -0.25f),         // 29
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec3(+0.25f, -0.25f, +1.00f),         // 30
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		vec3(+0.25f, +0.25f, +1.00f),         // 31
		vec3(+0.60f, +1.00f, +0.00f),		  // Color
		vec3(+1.00f, +0.00f, +0.00f),         // Normal
		// Left side of back of arrow
		vec3(-0.25f, +0.25f, -0.25f),         // 32
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec3(-0.25f, -0.25f, -0.25f),         // 33
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec3(-0.25f, -0.25f, +1.00f),         // 34
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		vec3(-0.25f, +0.25f, +1.00f),         // 35
		vec3(+0.00f, +1.00f, +0.00f),		  // Color
		vec3(-1.00f, +0.00f, +0.00f),         // Normal
		// Back side of back of arrow
		vec3(-0.25f, +0.25f, +1.00f),         // 36
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec3(+0.25f, +0.25f, +1.00f),         // 37
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec3(-0.25f, -0.25f, +1.00f),         // 38
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
		vec3(+0.25f, -0.25f, +1.00f),         // 39
		vec3(+0.50f, +0.50f, +0.50f),		  // Color
		vec3(+0.00f, +0.00f, +1.00f),         // Normal
	};

	GLushort stackIndices[] = {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
	};

	GLuint sVsize = sizeof(stackVerts) / sizeof(stackVerts[0]);
	Vertex * vertices = new Vertex[sVsize / 3];
	Color * colors = new Color[sVsize / 3];

	Vertex *vertex = nullptr;
	// ignore the normal values in stackVerts
	for (GLuint i = 0; i < sVsize; ++i) {
		if (i % 3 == 0) {
			vertex = &vertices[i / 3];
			vertex->position = stackVerts[i];
		}
		else if (i % 3 == 1)
			colors[(i - 1) / 3].color = stackVerts[i];
		else
			vertex->normal = stackVerts[i];
	}

	arrow->numVertices = sVsize / 3;
	arrow->vertices = vertices;
	arrow->colors = colors;

	arrow->numIndices = ARR_SIZE(stackIndices);
	arrow->indices = new GLushort[arrow->numIndices];
	memcpy(arrow->indices, stackIndices, sizeof(stackIndices));

	return arrow;
}

ShapeData* ShapeGenerator::makePlaneVerts(GLint dimensions)
{
	ShapeData* ret = new ShapeData();

	ret->numVertices = dimensions * dimensions;
	int half = dimensions / 2;
	ret->vertices = new Vertex[ret->numVertices];
	ret->colors = new Color[ret->numVertices];

	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			Vertex& thisVert = ret->vertices[i * dimensions + j];
			Color& thisColor = ret->colors[i * dimensions + j];
			thisVert.position.x = GLfloat(j - half);
			thisVert.position.z = GLfloat(i - half);
			thisVert.position.y = 0.0f;
			thisVert.normal = vec3(0.0f, 1.0f, 0.0f);
			thisColor.color = randomColor();
		}
	}
	return ret;
}

ShapeData* ShapeGenerator::makePlaneIndices(GLint dimensions)
{
	ShapeData* ret = new ShapeData();
	ret->numIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret->indices = new GLushort[ret->numIndices];
	int runner = 0;
	for (int row = 0; row < dimensions - 1; row++)
	{
		for (int col = 0; col < dimensions - 1; col++)
		{
			ret->indices[runner++] = dimensions * row + col;
			ret->indices[runner++] = dimensions * row + col + dimensions;
			ret->indices[runner++] = dimensions * row + col + dimensions + 1;
			
			ret->indices[runner++] = dimensions * row + col;
			ret->indices[runner++] = dimensions * row + col + dimensions + 1;
			ret->indices[runner++] = dimensions * row + col + 1;
		}
	}
	assert(runner == ret->numIndices);
	return ret;
}

ShapeData* ShapeGenerator::makePlane(GLint dimensions)
{
	ShapeData *ret = makePlaneVerts(dimensions);
	ShapeData *ret2 = makePlaneIndices(dimensions);
	ret->numIndices = ret2->numIndices;
	ret->indices = ret2->indices;

	cleanUpTemp(ret2);

	return ret;
}

ShapeData* ShapeGenerator::makeSphere(GLint tesselation)
{
	ShapeData *ret = makePlaneVerts(tesselation);
	ShapeData *ret2 = makePlaneIndices(tesselation);

	ret->indices = ret2->indices;
	ret->numIndices = ret2->numIndices;

	cleanUpTemp(ret2);

	GLint dimensions = tesselation;
	const GLfloat RADIUS = 1.0f;
	const GLfloat PI = 3.142f;
	const GLfloat CIRCLE = PI * 2;
	const GLfloat SLICE_ANGLE = CIRCLE / (dimensions - 1);
	for (GLint col = 0; col < dimensions; col++)
	{
		GLfloat phi = -SLICE_ANGLE * col;
		for (GLint row = 0; row < dimensions; row++)
		{
			GLfloat theta = GLfloat(-(SLICE_ANGLE / 2.0) * row);
			GLint vertIndex = col * dimensions + row;
			Vertex& v = ret->vertices[vertIndex];
			v.position.x = RADIUS * cos(phi) * sin(theta);
			v.position.y = RADIUS * sin(phi) * sin(theta);
			v.position.z = RADIUS * cos(theta);
		}
	}
	return ret;
}

ShapeData* ShapeGenerator::makePlaneUnseamedIndices(GLint tesselation)
{
	ShapeData* ret = new ShapeData();

	GLint dimensions = tesselation * tesselation;
	ret->numIndices = dimensions * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret->indices = new GLushort[ret->numIndices];
	int runner = 0;
	for (GLint row = 0; row < tesselation; row++)
	{
		// This code is crap but works, and I'm not in the mood right now to clean it up
		for (GLint col = 0; col < tesselation; col++)
		{
			// Bottom left triangle
			ret->indices[runner++] = tesselation * row + col;
			// One row down unless it's the bottom row, 
			ret->indices[runner++] = (row + 1 == tesselation ? 0 : tesselation * row + tesselation) + col;
			// Move to vert right of this one unless it's the last vert,
			// which we connect to the first vert in the row
			// the % dimensions at the end accounts for the last row hooking to the first row
			ret->indices[runner++] = (tesselation * row + col + tesselation + (col + 1 == tesselation ? -tesselation + 1 : 1)) % dimensions;

			// Upper right triangle
			ret->indices[runner++] = tesselation * row + col;
			if (col + 1 == tesselation && row + 1 == tesselation)
			{
				// Very last vert
				ret->indices[runner++] = 0;
			}
			else if (col + 1 == tesselation)
			{
				// Last vert on this row
				// Check if we need to connect it to zeroeth row or the next row
				if (row + 1 == tesselation)
				{
					// Tie to zeroeth row
					ret->indices[runner++] = col + 1;
				}
				else
				{
					// Tie to next row
					ret->indices[runner++] = tesselation * row + col + 1;
				}
			}
			else
			{
				// Regular interior vert
				// the % dimensions at the end accounts for the last row hooking to the first row
				ret->indices[runner++] = (tesselation * row + col + tesselation + 1) % dimensions;
			}
			ret->indices[runner++] = tesselation * row + col + (col + 1 == tesselation ? -col : 1);
		}
	}
	return ret;
}

ShapeData* ShapeGenerator::makeTorus(GLint tesselation)
{
	ShapeData* ret = new ShapeData();
	GLint dimensions = tesselation * tesselation;
	ret->numVertices = dimensions;
	ret->vertices = new Vertex[ret->numVertices];
	ret->colors = new Color[ret->numVertices];

	float sliceAngle = 360 / tesselation;
	const float torusRadius = 1.0f;
	const float pipeRadius = 0.5f;
	for (GLint round1 = 0; round1 < tesselation; round1++)
	{
		// Generate a circle on the xy plane, then
		// translate then rotate it into position
		glm::mat4 transform = glm::rotate(glm::mat4(1.0), round1 * sliceAngle, glm::vec3(0.0f, 1.0f, 0.0f)) *
							  glm::translate(glm::mat4(1.0), glm::vec3(torusRadius, 0.0f, 0.0f));
		glm::mat3 normalTransform = (glm::mat3)transform;
		for (GLint round2 = 0; round2 < tesselation; round2++)
		{
			Vertex& v = ret->vertices[round1 * tesselation + round2];
			Color& c = ret->colors[round1 * tesselation + round2];
			glm::vec4 glmVert(
				pipeRadius * cos(glm::radians(sliceAngle * round2)),
				pipeRadius * sin(glm::radians(sliceAngle * round2)),
				0,
				1.0f);
			glm::vec4 glmVertPrime = transform * glmVert;
			v.position = (glm::vec3)glmVertPrime;
			v.normal = glm::normalize(normalTransform * (glm::vec3)glmVert);
			c.color = randomColor();
		}
	}

	ShapeData* ret2 = makePlaneUnseamedIndices(tesselation);
	ret->numIndices = ret2->numIndices;
	ret->indices = ret2->indices;

	cleanUpTemp(ret2);

	return ret;
}

ShapeData* ShapeGenerator::makeTeapot(GLint tesselation, const glm::mat4& lidTransform)
{
	ShapeData* ret = new ShapeData();

	ret->numVertices = 32 * (tesselation + 1) * (tesselation + 1);
	GLuint faces = tesselation * tesselation * 32;
	GLfloat* vertices = new GLfloat[ret->numVertices * 3];
	GLfloat* normals = new GLfloat[ret->numVertices * 3];
	GLfloat* textureCoordinates = new float[ret->numVertices * 2];
	ret->numIndices = faces * 6;
	ret->indices = new GLushort[ret->numIndices];

	generatePatches(vertices, normals, textureCoordinates, ret->indices, tesselation);
	moveLid(tesselation, vertices, lidTransform);

	// Adapt/convert their data format to mine
	ret->vertices = new Vertex[ret->numVertices];
	ret->colors = new Color[ret->numVertices];

	for (GLuint i = 0; i < ret->numVertices; i++)
	{
		Vertex& v = ret->vertices[i];
		Color& c = ret->colors[i];
		v.position.x = vertices[i * 3 + 0];
		v.position.y = vertices[i * 3 + 1];
		v.position.z = vertices[i * 3 + 2];
		v.normal.x = normals[i * 3 + 0];
		v.normal.y = normals[i * 3 + 1];
		v.normal.z = normals[i * 3 + 2];
		c.color = randomColor();
	}

	return ret;
}

void ShapeGenerator::moveLid(int grid, float* v, const glm::mat4 &lidTransform) {

	int start = 3 * 12 * (grid + 1) * (grid + 1);
	int end = 3 * 20 * (grid + 1) * (grid + 1);

	for (int i = start; i < end; i += 3)
	{
		glm::vec4 vert = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
		vert = lidTransform * vert;
		v[i] = vert.x;
		v[i + 1] = vert.y;
		v[i + 2] = vert.z;
	}
}

void ShapeGenerator::generatePatches(float* v, float* n, float* tc, unsigned short* el, int grid) {
	float* B = new float[4 * (grid + 1)];  // Pre-computed Bernstein basis functions
	float* dB = new float[4 * (grid + 1)]; // Pre-computed derivitives of basis functions

	int idx = 0, elIndex = 0, tcIndex = 0;

	// Pre-compute the basis functions  (Bernstein polynomials)
	// and their derivatives
	computeBasisFunctions(B, dB, grid);

	// Build each patch
	// The rim
	buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The body
	buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The lid
	buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The bottom
	buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The handle
	buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	// The spout
	buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

	delete[] B;
	delete[] dB;
}

void ShapeGenerator::buildPatchReflect(int patchNum,
	float* B, float* dB,
	float* v, float* n,
	float* tc, unsigned short* el,
	int& index, int& elIndex, int& tcIndex, int grid,
	bool reflectX, bool reflectY)
{
	glm::vec3 patch[4][4];
	glm::vec3 patchRevV[4][4];
	getPatch(patchNum, patch, false);
	getPatch(patchNum, patchRevV, true);

	// Patch without modification
	buildPatch(patch, B, dB, v, n, tc, el,
		index, elIndex, tcIndex, grid, glm::mat3(1.0f), true);

	// Patch reflected in x
	if (reflectX) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in y
	if (reflectY) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)), false);
	}

	// Patch reflected in x and y
	if (reflectX && reflectY) {
		buildPatch(patch, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f)), true);
	}
}

#include "TeapotData.h"

void ShapeGenerator::getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV)
{
	for (int u = 0; u < 4; u++) {          // Loop in u direction
		for (int v = 0; v < 4; v++) {     // Loop in v direction
			if (reverseV) {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + (3 - v)]][2]
				);
			}
			else {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u * 4 + v]][2]
				);
			}
		}
	}
}

void ShapeGenerator::buildPatch(glm::vec3 patch[][4],
	float* B, float* dB,
	float* v, float* n, float* tc,
	unsigned short* el,
	int& index, int& elIndex, int& tcIndex, int grid, glm::mat3 reflect,
	bool invertNormal)
{
	int startIndex = index / 3;
	float tcFactor = 1.0f / grid;

	for (int i = 0; i <= grid; i++)
	{
		for (int j = 0; j <= grid; j++)
		{
			glm::vec3 pt = reflect * evaluate(i, j, B, patch);
			glm::vec3 norm = reflect * evaluateNormal(i, j, B, dB, patch);
			if (invertNormal)
				norm = -norm;

			v[index] = pt.x;
			v[index + 1] = pt.y;
			v[index + 2] = pt.z;

			n[index] = norm.x;
			n[index + 1] = norm.y;
			n[index + 2] = norm.z;

			tc[tcIndex] = i * tcFactor;
			tc[tcIndex + 1] = j * tcFactor;

			index += 3;
			tcIndex += 2;
		}
	}

	for (int i = 0; i < grid; i++)
	{
		int iStart = i * (grid + 1) + startIndex;
		int nextiStart = (i + 1) * (grid + 1) + startIndex;
		for (int j = 0; j < grid; j++)
		{
			el[elIndex] = iStart + j;
			el[elIndex + 1] = nextiStart + j + 1;
			el[elIndex + 2] = nextiStart + j;

			el[elIndex + 3] = iStart + j;
			el[elIndex + 4] = iStart + j + 1;
			el[elIndex + 5] = nextiStart + j + 1;

			elIndex += 6;
		}
	}
}

glm::vec3 ShapeGenerator::evaluate(int gridU, int gridV, float* B, glm::vec3 patch[][4])
{
	glm::vec3 p(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
		}
	}
	return p;
}

glm::vec3 ShapeGenerator::evaluateNormal(int gridU, int gridV, float* B, float* dB, glm::vec3 patch[][4])
{
	glm::vec3 du(0.0f, 0.0f, 0.0f);
	glm::vec3 dv(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
			dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
		}
	}
	return glm::normalize(glm::cross(du, dv));
}

void ShapeGenerator::computeBasisFunctions(float* B, float* dB, int grid) {
	float inc = 1.0f / grid;
	for (int i = 0; i <= grid; i++)
	{
		float t = i * inc;
		float tSqr = t * t;
		float oneMinusT = (1.0f - t);
		float oneMinusT2 = oneMinusT * oneMinusT;

		B[i * 4 + 0] = oneMinusT * oneMinusT2;
		B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
		B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
		B[i * 4 + 3] = t * tSqr;

		dB[i * 4 + 0] = -3.0f * oneMinusT2;
		dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
		dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
		dB[i * 4 + 3] = 3.0f * tSqr;
	}
}