#pragma once

#include "ShapeData.h"

class ShapeGenerator
{	
private:
	ShapeGenerator() {};

	static ShapeData* makePlaneVerts(GLuint dimensions);
	static ShapeData* makePlaneIndices(GLuint dimensions);

public:
	static ShapeData* makeTriangle();
	static ShapeData* makeCube();
	static ShapeData* makeArrow();
	static ShapeData* makeCube();
};
