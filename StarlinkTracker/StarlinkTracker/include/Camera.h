#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>;
#include "InputManager.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void ProcessKeyboardInput(GLfloat deltaTime);
	void ProcessMouseInput(GLfloat xChange, GLfloat yChange);
	glm::mat4 GetViewMatrix();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat mouseSensitivity;

	void UpdateCameraVectors();
};

#endif
