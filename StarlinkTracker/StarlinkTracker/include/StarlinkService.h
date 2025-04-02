#ifndef STARLINKSERVICE_HPP
#define STARLINKSERVICE_HPP

#include <vector>
#include <string>

void disableStarlink(int starlinkId);

void enableStarlink(int starlinkId);

void highlightStarlink(int starlinkId);

void toggleCountriesBorder(bool* isCountriesBorderVisible);

void showTrajectory(int starlinkId);

void rotateCamera(float angleX, float angleY, float angleZ);

#endif // STARLINKSERVICE_HPP
