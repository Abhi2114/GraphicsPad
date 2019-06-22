#include "Camera.h"
#include <gtx/transform.hpp>

Camera::Camera():MOVEMENT_SPEED(0.1f)
{
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	eye = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getWorldToViewMatrix() const {

	return glm::lookAt(eye, eye + viewDirection, up);
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition) {

	glm::vec2 delta = newMousePosition - oldMousePosition;
	const float ROTATIONAL_SPEED = 0.5f;

	if (glm::length(delta) < 50.0f) {

		// rotate viewDirection and up along the new axis
		glm::mat4 rotateVertical = glm::rotate(glm::radians(-delta.y) * ROTATIONAL_SPEED, glm::cross(viewDirection, up));

		viewDirection =  rotateVertical * glm::vec4(viewDirection, 1.0f);
		up = rotateVertical * glm::vec4(up, 1.0f);

		// rotate the view about the new up vector
		viewDirection = glm::rotate(glm::radians(-delta.x) * ROTATIONAL_SPEED, up) * 
						glm::vec4(viewDirection, 1.0f);
	}

	// update the old mouse position
	oldMousePosition = newMousePosition;
}

void Camera::moveForward() {

	// move the camera forward
	eye += MOVEMENT_SPEED * viewDirection;
}

void Camera::moveBackward() {

	// reverse the vector to move backward
	eye += -MOVEMENT_SPEED * viewDirection;
}

void Camera::strafeLeft() {

	// move to the left along the vector which is obtained by taking the cross product of 
	// the viewDirection and up vector
	eye += MOVEMENT_SPEED * glm::cross(up, viewDirection);
}

void Camera::strafeRight() {

	eye += MOVEMENT_SPEED * glm::cross(viewDirection, up);
}

void Camera::moveUp() {
	
	eye += MOVEMENT_SPEED * up;
}

void Camera::moveDown() {

	eye += -MOVEMENT_SPEED * up;
}