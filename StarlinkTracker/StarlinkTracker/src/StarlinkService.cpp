#include "../include/StarlinkService.h"
#include <iostream>

void disableStarlink(int starlinkId) {
    std::cout << "Disabling Starlink with ID: " << starlinkId << std::endl;
}

void enableStarlink(int starlinkId) {
    std::cout << "Enabling Starlink with ID: " << starlinkId << std::endl;
}

void highlightStarlink(int starlinkId) {
    std::cout << "Highlighting Starlink with ID: " << starlinkId << std::endl;
}

void toggleCountriesBorder(bool* isCountriesBorderVisible) {
    *isCountriesBorderVisible = !(*isCountriesBorderVisible);
    std::cout << "Countries border visibility: " << (*isCountriesBorderVisible ? "Enabled" : "Disabled") << std::endl;
}

void showTrajectory(int starlinkId) {
    std::cout << "Showing trajectory for Starlink with ID: " << starlinkId << std::endl;
}

void rotateCamera(float angleX, float angleY, float angleZ) {
    std::cout << "Rotating camera with angles: X = " << angleX << ", Y = " << angleY << ", Z = " << angleZ << std::endl;
}
