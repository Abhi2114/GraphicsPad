#pragma once

#include <glm.hpp>

class Camera
{
	glm::vec3 eye;
	glm::vec3 viewDirection;
	glm::vec2 oldMousePosition;
	glm::vec3 up;
	const float MOVEMENT_SPEED;

public:
	Camera();
	glm::mat4 getWorldToViewMatrix() const;
	void mouseUpdate(const glm::vec2& newMousePosition);

	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
};
