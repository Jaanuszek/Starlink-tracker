
#include "../include/Camera.h"
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	: position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(startMoveSpeed), mouseSensitivity(startTurnSpeed), target(glm::vec3(0.0f, 0.0f, 0.0f)) {
	UpdateCameraVectors();
	UpdateCameraVectorsBaseOnEndpoint();
	defaultLookAt = glm::lookAt(position, position + front, up);;
	currentLookAt = defaultLookAt;
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

glm::highp_mat4 Camera::GetViewMatrix() {
	//return glm::lookAt(position, position + front, up);
	return currentLookAt;
}

void Camera::setStarlinkToHiglight(const Starlink& starlink)
{
	higlighted_starlink = &starlink;
}

void Camera::highlightStarlink()
{
	//Satellite starlinkInfo = starlink.getSatelliteInfo();
	//higlighted_starlink = &starlink;
	glm::vec3 pos = higlighted_starlink->changeCoordsToSphere();
	//std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
	pos = glm::vec3(pos.x/100.0f, pos.y/100.0f, pos.z/100.0f);
	//satelliteInfo.latitude = geodetic.latitude * (180.0f / M_PI);
	//satelliteInfo.longitude = geodetic.longitude * (180.0f / M_PI);
	//satelliteInfo.altitude = geodetic.altitude;
	//float yawRad = glm::radians(yaw);
	//float pitchRad = glm::radians(pitch);
	glm::vec3 temp_target = pos;
	glm::vec3 temp_position;

	glm::vec3 offset(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraPos = pos + offset;
	//glm::vec3 cameraPos = position;


	//temp_position.x = target.x + 2 * cos(pitchRad) * cos(yawRad);
	//temp_position.y = target.y + 2 * sin(pitchRad);
	//temp_position.z = target.z + 2 * cos(pitchRad) * sin(yawRad);
	glm::highp_vec3 temp_front = glm::normalize(pos - cameraPos);
	glm::highp_vec3 temp_right = glm::normalize(glm::cross(temp_front, worldUp));
	glm::highp_vec3 temp_up = glm::normalize(glm::cross(temp_right, temp_front));
	//currentLookAt = glm::lookAt(cameraPos, pos + glm::vec3(0.0f,0.0f,-1.0f), up);
	currentLookAt = glm::lookAt(cameraPos, pos, up);
}

void Camera::setDefaultViewPort()
{
	currentLookAt = defaultLookAt;
}