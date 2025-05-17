#include "../../include/Models/Starlink.h"

Starlink::Starlink(const Satellite& satelliteInfo, const std::tm& local_time)
    : satelliteInfo(satelliteInfo), sgp4(libsgp4::Tle(satelliteInfo.satname, satelliteInfo.tleLine1, satelliteInfo.tleLine2)),
    startTime(1900 + local_time.tm_year, 1 + local_time.tm_mon, local_time.tm_mday, local_time.tm_hour, local_time.tm_min, local_time.tm_sec),
    eci(sgp4.FindPosition(startTime)), geodetic(eci.ToGeodetic())
{
    libsgp4::Vector position = eci.Position();
    libsgp4::Vector velocity = eci.Velocity();
    cartesianStarlinkPosition = glm::vec3(position.x, position.y, position.z);
    std::cout << "Satellite name: " << satelliteInfo.satname << std::endl;
    std::cout << "Position (km): x = " << position.x << ", y = " << position.y << ", z = " << position.z << std::endl;
    std::cout << "Velocity (km/s): x = " << velocity.x << ", y = " << velocity.y << ", z = " << velocity.z << std::endl;

    this->satelliteInfo.latitude = geodetic.latitude * (180.0f / M_PI);
    this->satelliteInfo.longitude = geodetic.longitude * (180.0f / M_PI);
    this->satelliteInfo.altitude = geodetic.altitude;

    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    model = glm::translate(model, changeCoordsToSphere());

    trajectoryLine.resize(1000);
    starlinkTrajectoryLine = std::make_unique<Mesh>(trajectoryLine, true);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.2f, 1.0f);
    trajectoryLineColor = { dis(gen), dis(gen), dis(gen), dis(gen) };
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
    model = glm::translate(model, changeCoordsToSphere());
}

glm::vec3 Starlink::changeCoordsToSphere() const 
{
    float theta = satelliteInfo.longitude * M_PI / 180.0f;
    float phi = (90.0f - satelliteInfo.latitude) * M_PI / 180.0f;
    float x = radius * sin(phi) * cos(theta);
    float y = radius * cos(phi);
    float z = radius * sin(phi) * sin(theta);
    return { x, y, z };
}

glm::vec3 Starlink::changeCoordsToSphere(float lon, float lat, float radius) const
{
    float theta = lon * M_PI / 180.0f;
    float phi = (90.0f - lat) * M_PI / 180.0f;
    float x = radius * sin(phi) * cos(theta);
    float y = radius * cos(phi);
    float z = radius * sin(phi) * sin(theta);
    return { x, y, z };
}

// We dont use this function, but I will keep it here for now
void Starlink::createTrajectoryLine() {
    float period; // in seconds
    // 6378 is only radius of equator. In case of any problems with trajectory line, get back here
    float orbitalRadius = 6378.1 + satelliteInfo.altitude; // earth radius plus satellite altitude [KM]
    float earthGravitationalConstant = 398600.4418; // [km^3/s^2]
    period = 2 * M_PI * sqrt((pow(orbitalRadius, 3)) / earthGravitationalConstant);
    for (int i = 0; i < 1000; i++)
    {
        libsgp4::DateTime currentTime = startTime.AddSeconds(i * period / 100);
        libsgp4::Eci eci = sgp4.FindPosition(currentTime);
        libsgp4::CoordGeodetic geodetic = eci.ToGeodetic();
        float latitude = geodetic.latitude * (180.0f / M_PI);
        float longitude = geodetic.longitude * (180.0f / M_PI);
        VertexPosOnly vertex;
        vertex.position = changeCoordsToSphere(latitude, longitude, radius);
        trajectoryLine[i] = vertex;
    }
}

void Starlink::saveStarlinkPositionInVector()
{
    VertexPosOnly vertex;
    vertex.position = changeCoordsToSphere();
    if (savedTrajectoryCoords < trajectoryLine.size())
    {
        trajectoryLine[savedTrajectoryCoords] = vertex;
        savedTrajectoryCoords++;
    }
    else
    {
        vertex.position = { 0.0f, 0.0f, 0.0f };
        trajectoryLine.resize(1000, vertex);
        savedTrajectoryCoords = 0;
    }
    starlinkTrajectoryLine->UpdateData(trajectoryLine);
}

void Starlink::DrawTrajectory() {
    starlinkTrajectoryLine->DrawWithoutEBO(GL_LINE_STRIP, savedTrajectoryCoords % trajectoryLine.size());
}