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
    libsgp4::SGP4 sgp4;
    libsgp4::DateTime startTime;
    libsgp4::Eci eci;
    libsgp4::CoordGeodetic geodetic;
    float radius = 100.0f;
    std::unique_ptr<Mesh> starlinkTrajectoryLine;
    unsigned int savedTrajectoryCoords = 0;

    glm::vec3 changeCoordsToSphere();
    glm::vec3 changeCoordsToSphere(float lon, float lat, float radius);
    void createTrajectoryLine();
public:
    Starlink(const Satellite& satelliteInfo, const std::tm& local_time);
    ~Starlink();
    void UpdatePosition(float elapsedSeconds);
    void DrawTrajectory();
    void saveStarlinkPositionInVector();
    const glm::mat4& getModelMatrix() const { return model; }
    const std::vector<VertexPosOnly>& getTrajectoryLine() const { return trajectoryLine; }
    const glm::vec4& getTrajectoryLineColor() const { return trajectoryLineColor; }
};

#endif
