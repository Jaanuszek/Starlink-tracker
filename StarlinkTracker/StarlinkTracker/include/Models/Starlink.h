#ifndef STARLINK_H
#define STARLINK_H

#include "../JSONParser.h"
#include "../Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <memory>
#include <random>

class Starlink {
private:
    Satellite satelliteInfo;
    std::vector <VertexPosOnly> trajectoryLine;
    glm::vec4 trajectoryLineColor;
    glm::mat4 model;
    glm::vec3 cartesianStarlinkPosition;
    libsgp4::SGP4 sgp4;
    libsgp4::DateTime startTime;
    libsgp4::Eci eci;
    libsgp4::CoordGeodetic geodetic;
    float radius = 100.0f;
    std::unique_ptr<Mesh> starlinkTrajectoryLine;
    unsigned int savedTrajectoryCoords = 0;
    void createTrajectoryLine();
public:
    Starlink(const Satellite& satelliteInfo, const std::tm& local_time);
    ~Starlink();
    void UpdatePosition(float elapsedSeconds);
    void DrawTrajectory();
    void saveStarlinkPositionInVector();
    const glm::mat4& getModelMatrix() const { return model; }
    const std::vector<VertexPosOnly>& getTrajectoryLine() const { return trajectoryLine; }
    const glm::vec3& getCartesianPos() const { return cartesianStarlinkPosition; }
    const glm::vec4& getTrajectoryLineColor() const { return trajectoryLineColor; }
    const Satellite& getSatelliteInfo() const { return satelliteInfo; }
    libsgp4::Vector getCurrentPosition() const { return eci.Position(); }
    libsgp4::Vector getCurrentVelocity() const { return eci.Velocity(); }
    glm::vec3 changeCoordsToSphere() const;
    glm::vec3 changeCoordsToSphere(float lon, float lat, float radius) const;
};

#endif