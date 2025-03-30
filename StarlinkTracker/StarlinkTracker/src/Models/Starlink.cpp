#include "../../include/Models/Starlink.h"

Starlink::Starlink(const Satellite& satelliteInfo)
    : satelliteInfo(satelliteInfo)
{
    JSONParser jsonParser;
    model = glm::mat4(1.0f);
    model = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
    model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, jsonParser.changeCoordsToSphere(satelliteInfo.longitude, satelliteInfo.latitude, 100.0f));
}

Starlink::~Starlink() {}