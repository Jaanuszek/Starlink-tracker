#ifndef STARLINK_H
#define STARLINK_H

#include "../JSONParser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Starlink {
private:
    Satellite satelliteInfo;
    glm::mat4 model;
public:
    Starlink(const Satellite& satelliteInfo);
    ~Starlink();
    const glm::mat4& getModelMatrix() const { return model; }
};

#endif
