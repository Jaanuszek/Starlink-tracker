#ifndef STARLINK_H
#define STARLINK_H

#include "../JSONParser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Starlink {
private:
    Satellite satelliteInfo;
    glm::mat4 model;
    libsgp4::SGP4 sgp4;
    libsgp4::DateTime startTime;
    libsgp4::Eci eci;
    libsgp4::CoordGeodetic geodetic;
    float radius = 100.0f;

    glm::vec3 changeCoordsToSphere();
public:
    Starlink(const Satellite& satelliteInfo);
    ~Starlink();
    void UpdatePosition(float elapsedSeconds);
    const glm::mat4& getModelMatrix() const { return model; }
};

#endif
