#pragma once

#include "ShapeData.h"

class ShapeGenerator
{	
private:
	ShapeGenerator() {};
public:
	static ShapeData* makeTriangle();
	static ShapeData* makeCube();
	static ShapeData* makeArrow();

};
