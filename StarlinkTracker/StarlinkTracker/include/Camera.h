#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Models/Starlink.h"
#include <GLFW/glfw3.h>;
#include "InputManager.h"

//class Starlink;

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch,
		GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	~Camera();

	void ProcessKeyboardInput(GLfloat deltaTime);
	void ProcessMouseInput(GLfloat xChange, GLfloat yChange);
	void MoveCamera(GLfloat xChange, GLfloat yChange);
	glm::highp_mat4 GetViewMatrix();
	void setStarlinkToHiglight(const Starlink& starlink);
	void highlightStarlink();
	void setDefaultViewPort();

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

	glm::highp_mat4 defaultLookAt;
	glm::highp_mat4 currentLookAt;
	const Starlink* higlighted_starlink;

	glm::vec3 target;

	void UpdateCameraVectors();
	void UpdateCameraVectorsBaseOnEndpoint();
};

#endif
