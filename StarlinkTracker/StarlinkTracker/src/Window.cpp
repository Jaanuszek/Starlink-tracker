#include "../include/Window.h"

Window::Window()
    : width(800), height(600), mouseXDelta(0.0f), mouseYDelta(0.0f),
    firstMouseMove(true) {}

Window::Window(GLint windowWidth = 800, GLint windowHeight = 600)
    : width(windowWidth), height(windowHeight), mouseXDelta(0.0f), mouseYDelta(0.0f),
    firstMouseMove(true) {}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    std::cout << "[DEBUG] Window deleted" << std::endl;
}

int Window::Initialize() {
    if (!glfwInit()) {
        std::cerr << "[ERROR] GLFW initialization failed!" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr);
    if (!mainWindow) {
        std::cerr << "[ERROR] GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &frameBufferWidth, &frameBufferHeight);
    glfwMakeContextCurrent(mainWindow);
    CreateCallbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Initializing GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetWindowUserPointer(mainWindow, this);
    glfwSetFramebufferSizeCallback(mainWindow, OnFramebufferResize);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 0;
}

void Window::CreateCallbacks() {
    glfwSetKeyCallback(mainWindow, OnKeyInput);
    glfwSetCursorPosCallback(mainWindow, OnMouseMove);
    glfwSetMouseButtonCallback(mainWindow, OnMouseButtonClick);
}

void Window::ToggleCursorVisibility() {
    InputManager& input = InputManager::getInstance();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::OnFramebufferResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::OnKeyInput(GLFWwindow* window, int key, int code, int action, int mode) {
    InputManager& input = InputManager::getInstance();
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        input.setKey(key, action != GLFW_RELEASE);
    }
}

void Window::OnMouseMove(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->firstMouseMove) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->firstMouseMove = false;
    }

    theWindow->mouseXDelta = xPos - theWindow->lastX;
    theWindow->mouseYDelta = theWindow->lastY - yPos;
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

void Window::OnMouseButtonClick(GLFWwindow* window, int button, int action, int mods) {
    InputManager& input = InputManager::getInstance();
    input.setMouseButton(button, action == GLFW_PRESS);
}

GLfloat Window::GetMouseXDelta() {
    GLfloat delta = mouseXDelta;
    mouseXDelta = 0.0f;
    return delta;
}

GLfloat Window::GetMouseYDelta() {
    GLfloat delta = mouseYDelta;
    mouseYDelta = 0.0f;
    return delta;
}

void Window::SetWindowTitle(const std::string& title) {
    glfwSetWindowTitle(mainWindow, title.c_str());
}

float Window::CountFPSandMS(double& previousTime, double& currentTime, double timeDiff, unsigned int& frameCounter) {
    std::string FPS_string;
    std::string ms;
    std::string newTitle;
    float FPSCount = 0.0;
    if (timeDiff >= 1.0 / 30.0)
    {
        FPSCount = frameCounter / timeDiff;
        FPS_string = std::to_string(FPSCount);
        ms = std::to_string((timeDiff / frameCounter) * 1000);
        newTitle = "StarlinkTracker: " + FPS_string + "FPS, " + ms + " ms/frame";
        SetWindowTitle(newTitle);
        frameCounter = 0;
        previousTime = currentTime;
    }
    return FPSCount;
}