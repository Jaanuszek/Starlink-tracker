#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <iostream>  

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; };
	GLfloat getBufferHeight() { return bufferHeight; };
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); };
	bool* getKeys() { return keys; };
	GLfloat getXChange();
	GLfloat getYChange();
	void SwapBuffers() { glfwSwapBuffers(mainWindow); };

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mousedFirstMoved;

	void createCallbacks();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

#endif
