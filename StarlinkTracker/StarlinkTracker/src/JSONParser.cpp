#include "../include/JSONParser.h"

PrimitiveType JSONParser::getPrimitiveType(const nlohmann::json& parsedData)
{
    std::string type = parsedData["type"];
    if (type == "Point")
    {
        return PrimitiveType::POINT;
    }
    else if (type == "LineString")
    {
        return PrimitiveType::LINESTRING;
    }
    else if (type == "Polygon")
    {
        return PrimitiveType::POLYGON;
    }
    else if (type == "MultiPoint")
    {
        return PrimitiveType::MULTIPOINT;
    }
    else if (type == "MultiLineString")
    {
        return PrimitiveType::MULTILINESTRING;
    }
    else if (type == "MultiPolygon")
    {
        return PrimitiveType::MULTIPOLYGON;
    }
    else if (type == "GeometryCollection")
    {
        return PrimitiveType::GEOMETRYCOLLECTION;
    }
    else
    {
        return PrimitiveType::NONE;
    }
}
glm::vec3 JSONParser::changeCoordsToSphere(float lon, float lat, float radius) {
    float theta = lon * M_PI / 180.0f;
    float phi = (90.0f - lat) * M_PI / 180.0f;
    float x = radius * sin(phi) * cos(theta);
    float y = radius * cos(phi);
    float z = radius * sin(phi) * sin(theta);

    return { x, y, z };
}

std::map<unsigned int, std::vector<VertexPosOnly>> JSONParser::getVertex(const nlohmann::json& parsedData, PrimitiveType primType, float radius)
{
    std::map<unsigned int, std::vector<VertexPosOnly>> countriesVertices;

    if (!parsedData.contains("coordinates") || !parsedData["coordinates"].is_array())
    {
        std::cerr << "[ERROR] Coordinates are missing or not an array!" << std::endl;
        return countriesVertices;
    }

    VertexPosOnly vertex;
    int polyIndex = 0;

    if (primType == PrimitiveType::POLYGON)
    {
        for (const auto& polygon : parsedData["coordinates"])
        {
            std::vector<VertexPosOnly> vertices;
            for (const auto& point : polygon)
            {
                if (point.size() >= 2) {
                    vertex.position = changeCoordsToSphere(point[0], point[1], radius);
                    vertices.push_back(vertex);
                }
            }
            countriesVertices[polyIndex++] = vertices;
        }
    }
    else if (primType == PrimitiveType::MULTIPOLYGON)
    {
        for (const auto& multipolygon : parsedData["coordinates"])
        {
            for (const auto& ring : multipolygon)
            {
                std::vector<VertexPosOnly> vertices; // :)))))
                for (const auto& point : ring)
                {
                    if (point.size() >= 2) {
                        vertex.position = changeCoordsToSphere(point[0], point[1], radius);
                        vertices.push_back(vertex);
                    }
                }
                countriesVertices[polyIndex++] = vertices;
            }
        }
    }
    else
    {
        std::cerr << "[ERROR] Primitive type is NONE!" << std::endl;
    }
    return countriesVertices;
}

JSONParser::JSONParser() {}

JSONParser::~JSONParser() {}

void JSONParser::ParseJSONSattelite(const std::string& satData, std::vector<Satellite>& satellites)
{
    try {
        nlohmann::json parsedData = nlohmann::json::parse(satData);

        if (parsedData.contains("info") && parsedData["info"].contains("satid") && parsedData["info"].contains("satname") && parsedData["info"].contains("transactionscount") && parsedData.contains("tle")) {
            const auto& info = parsedData["info"];
            Satellite satellite;
            satellite.satid = info["satid"].get<int>();
            satellite.satname = info["satname"].get<std::string>();
            satellite.transactionscount = info["transactionscount"].get<int>();

            if (parsedData["tle"].is_string()) {
                std::string tle = parsedData["tle"].get<std::string>();
                if (tle.empty())
                {
                    std::cout << "[ERROR] TLE field is empty!" << std::endl;
                    return;
                }
                size_t splitPos = tle.find("\r\n");
                if (splitPos != std::string::npos) {
                    satellite.tleLine1 = tle.substr(0, splitPos);
                    satellite.tleLine2 = tle.substr(splitPos + 2);
                }
                else {
                    std::cout << "[ERROR] TLE line separator not found!" << std::endl;
                }
                std::cout << satellite.satname << std::endl;
                std::cout << satellite.tleLine1 << std::endl;
                std::cout << satellite.tleLine2 << std::endl;

                libsgp4::SGP4 sgp4(libsgp4::Tle(satellite.satname, satellite.tleLine1, satellite.tleLine2));
                libsgp4::DateTime dt(2025, 3, 17, 20, 0, 0);
                libsgp4::Eci eci = sgp4.FindPosition(dt);
                libsgp4::Vector position = eci.Position();
                libsgp4::Vector velocity = eci.Velocity();

                std::cout << "Position (km): x = " << position.x << ", y = " << position.y << ", z = " << position.z << std::endl;
                std::cout << "Velocity (km/s): x = " << velocity.x << ", y = " << velocity.y << ", z = " << velocity.z << std::endl;

                libsgp4::CoordGeodetic geodetic = eci.ToGeodetic();
                satellite.latitude = geodetic.latitude * (180.0f / M_PI); // Check if this whould be in degrees or radians
                satellite.longitude = geodetic.longitude * (180.0f / M_PI);
                satellite.altitude = geodetic.altitude;

                std::cout << "Latitude: " << satellite.latitude << std::endl;
                std::cout << "Longitude: " << satellite.longitude << std::endl;
                std::cout << "Altitude: " << satellite.altitude << std::endl;

                satellites.push_back(satellite);
            }
            else {
                std::cerr << "TLE field is missing or not a string!" << std::endl;
            }
        }
        else {
            std::cerr << "Required fields are missing in the response!" << std::endl;
        }
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}


void JSONParser::ParseGeoJSON(const char* pathToGeoJSON,float radius)
{
    std::ifstream GeoJSONfile(pathToGeoJSON);
    if (!GeoJSONfile.is_open())
    {
        std::cerr << "[ERROR] Failed to open GeoJSON file! Path: " << pathToGeoJSON << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << GeoJSONfile.rdbuf();
    const std::string parsedGeoData = buffer.str();
    try
    {
        const nlohmann::json parsedData = nlohmann::json::parse(parsedGeoData);
        if (!parsedData.contains("features") || !parsedData["features"].is_array())
        {
            std::cerr << "[ERROR] No features in GeoJSON file or not an array!" << std::endl;
            return;
        }
        for (const auto& feature : parsedData["features"])
        {
            if (!feature.contains("properties") || !feature.contains("geometry"))
            {
                continue;
            }
            Country country;
            const auto& propertiesData = feature["properties"];
            
            country.name = propertiesData.value("NAME", "Unknown");
            country.continent = propertiesData.value("CONTINENT", "Unknown");

            const auto& geometryData = feature["geometry"];
            PrimitiveType primitive = PrimitiveType::NONE;

            if (geometryData.contains("type"))
            {
                primitive = getPrimitiveType(geometryData);
            }
            
            std::map<unsigned int, std::vector<VertexPosOnly>> countryVertices;
            if (geometryData.contains("coordinates"))
            {
                countryVertices = getVertex(geometryData, primitive, radius);
            }
            countriesMap[country] = { primitive, countryVertices };
        }
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "[ERROR] JSON parsing error: " << e.what() << std::endl;
    }
}
