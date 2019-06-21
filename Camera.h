#pragma once

#include <glm.hpp>

class Camera
{
	glm::vec3 eye;
	glm::vec3 viewDirection;
	glm::vec2 oldMousePosition;
	float totalDegrees;
	const glm::vec3 UP;

public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
	void mouseUpdate(const glm::vec2& newMousePosition);
};
