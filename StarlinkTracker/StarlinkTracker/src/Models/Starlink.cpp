#include "../../include/Models/Starlink.h"

Starlink::Starlink(const Satellite& satelliteInfo)
    : satelliteInfo(satelliteInfo), sgp4(libsgp4::Tle(satelliteInfo.satname, satelliteInfo.tleLine1, satelliteInfo.tleLine2)),
    startTime(2025, 3, 31, 20, 0, 0), eci(sgp4.FindPosition(startTime)), geodetic(eci.ToGeodetic())
{
    libsgp4::Vector position = eci.Position();
    libsgp4::Vector velocity = eci.Velocity();
    std::cout << "Sattelite name: " << satelliteInfo.satname << std::endl;
    std::cout << "Position (km): x = " << position.x << ", y = " << position.y << ", z = " << position.z << std::endl;
    std::cout << "Velocity (km/s): x = " << velocity.x << ", y = " << velocity.y << ", z = " << velocity.z << std::endl;

    this->satelliteInfo.latitude = geodetic.latitude * (180.0f / M_PI);
    this->satelliteInfo.longitude = geodetic.longitude * (180.0f / M_PI);
    this->satelliteInfo.altitude = geodetic.altitude;

    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, changeCoordsToSphere());
    for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
        std::cout << model[i][j] << " ";
    }
    std::cout << std::endl;
    }
}

Starlink::~Starlink() {}

void Starlink::UpdatePosition(float elapsedSeconds) {
    libsgp4::DateTime currentTime = startTime.AddSeconds(elapsedSeconds);
    libsgp4::Eci eci = sgp4.FindPosition(currentTime);
    libsgp4::CoordGeodetic geodetic = eci.ToGeodetic();

    satelliteInfo.latitude = geodetic.latitude * (180.0f / M_PI);
    satelliteInfo.longitude = geodetic.longitude * (180.0f / M_PI);
    satelliteInfo.altitude = geodetic.altitude;

    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, changeCoordsToSphere());
}

glm::vec3 Starlink::changeCoordsToSphere() {
    float theta = satelliteInfo.longitude * M_PI / 180.0f;
    float phi = (90.0f - satelliteInfo.latitude) * M_PI / 180.0f;
    float x = radius * sin(phi) * cos(theta);
    float y = radius * cos(phi);
    float z = radius * sin(phi) * sin(theta);
    return { x, y, z };
}