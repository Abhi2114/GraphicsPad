#pragma once
#include <glm.hpp>

using glm::vec3;

struct Position {
	vec3 position;
};

struct Color {
	vec3 color;
};

struct Vertex
{
	vec3 position;
	vec3 color;
	vec3 normal;
};