
#include "../include/Camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	: position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(startMoveSpeed), mouseSensitivity(startTurnSpeed), target(glm::vec3(0.0f, 0.0f, 0.0f)) {
	UpdateCameraVectors();
	UpdateCameraVectorsBaseOnEndpoint();
}

Camera::~Camera() {
	std::cout << "[DEBUG] Camera deleted" << std::endl;
}

void Camera::UpdateCameraVectors() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::ProcessKeyboardInput(GLfloat deltaTime) {
	InputManager& input = InputManager::getInstance();
	GLfloat velocity = movementSpeed * deltaTime;

	if (input.isKeyPressed(GLFW_KEY_W)) {
		position += front * velocity;
	}

	if (input.isKeyPressed(GLFW_KEY_S)) {
		position -= front * velocity;
	}

	if (input.isKeyPressed(GLFW_KEY_A)) {
		position -= right * velocity;
	}

	if (input.isKeyPressed(GLFW_KEY_D)) {
		position += right * velocity;
	}

	if (input.isKeyPressed(GLFW_KEY_SPACE)) {
		position += worldUp * velocity;
	}

	if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		position -= worldUp * velocity;
	}
}

void Camera::ProcessMouseInput(GLfloat xChange, GLfloat yChange) {
	InputManager& input = InputManager::getInstance();
	if (!input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		return;
	}

	xChange *= mouseSensitivity;
	yChange *= mouseSensitivity;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectorsBaseOnEndpoint() {
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);

	position.x = target.x + 2 * cos(pitchRad) * cos(yawRad);
	position.y = target.y + 2 * sin(pitchRad);
	position.z = target.z + 2 * cos(pitchRad) * sin(yawRad);

	front = glm::normalize(target - position);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::MoveCamera(GLfloat xChange, GLfloat yChange) {
	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	UpdateCameraVectorsBaseOnEndpoint();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}