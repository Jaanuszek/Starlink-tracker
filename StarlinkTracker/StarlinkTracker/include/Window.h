#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <iostream>  
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "InputManager.h"

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialize();
	GLfloat GetFrameBufferWidth() { return frameBufferWidth; };
	GLfloat GetFrameBufferHeight() { return frameBufferHeight; };
	bool ShouldClose() { return glfwWindowShouldClose(mainWindow); };
	GLfloat GetMouseXDelta();
	GLfloat GetMouseYDelta();
	void SwapFrameBuffers() { glfwSwapBuffers(mainWindow); };
	void ToggleCursorVisibility();
	void SetWindowTitle(const std::string& title);
    float CountFPSandMS(double& previousTime, double& currentTime, double timeDiff, unsigned int& frameCounter);

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint frameBufferWidth, frameBufferHeight;

	GLfloat lastX;
	GLfloat lastY;
	GLfloat mouseXDelta;
	GLfloat mouseYDelta;
	bool firstMouseMove;

	void CreateCallbacks();
	static void OnFramebufferResize(GLFWwindow* window, int width, int height);
	static void OnKeyInput(GLFWwindow* window, int key, int code, int action, int mode);
	static void OnMouseMove(GLFWwindow* window, double xPos, double yPos);
	static void OnMouseButtonClick(GLFWwindow* window, int button, int action, int mods);
};

#endif
