// InputManager.h
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <GLFW/glfw3.h>

class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    void setKey(int key, bool isPressed) {
        keys[key] = isPressed;
    }

    bool isKeyPressed(int key) {
        return keys[key];
    }

    void setMouseButton(int button, bool isPressed) {
        mouseButtons[button] = isPressed;
    }

    bool isMouseButtonPressed(int button) {
        return mouseButtons[button];
    }

private:
    InputManager() {}
    ~InputManager() {}
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_map<int, bool> keys;
    std::unordered_map<int, bool> mouseButtons;
};

#endif