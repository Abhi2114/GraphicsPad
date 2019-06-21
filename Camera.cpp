#include "Camera.h"
#include <gtx/transform.hpp>
#include <iostream>

Camera::Camera(): UP(0.0f, 1.0f, 0.0f)
{
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	eye = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::mat4 Camera::getWorldToViewMatrix() const {

	return glm::lookAt(eye, viewDirection, UP);
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {

	glm::vec2 delta = newMousePosition - oldMousePosition;
	const float ROTATIONAL_SPEED = 0.5f;

	if (glm::length(delta) < 50.0f) {

		viewDirection = glm::rotate(glm::radians(-delta.x) * ROTATIONAL_SPEED, UP) * 
			glm::rotate(glm::radians(-delta.y) * ROTATIONAL_SPEED, glm::cross(viewDirection, UP)) * 
			glm::vec4(viewDirection, 1.0f);
	}

	// update the old mouse position
	oldMousePosition = newMousePosition;
}