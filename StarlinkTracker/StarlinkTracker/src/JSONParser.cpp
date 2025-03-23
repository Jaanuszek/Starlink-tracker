#include "../include/JSONParser.h"

primitiveType JSONParser::getPrimitiveType(const nlohmann::json& parsedData, unsigned int index)
{
	std::string type = parsedData["type"];
	if (type == "Point")
	{
		return primitiveType::POINT;
	}
	else if (type == "LineString")
	{
		return primitiveType::LINESTRING;
	}
	else if (type == "Polygon")
	{
		return primitiveType::POLYGON;
	}
	else if (type == "MultiPoint")
	{
		return primitiveType::MULTIPOINT;
	}
	else if (type == "MultiLineString")
	{
		return primitiveType::MULTILINESTRING;
	}
	else if (type == "MultiPolygon")
	{
		return primitiveType::MULTIPOLYGON;
	}
	else if (type == "GeometryCollection")
	{
		return primitiveType::GEOMETRYCOLLECTION;
	}
	else
	{
		return primitiveType::NONE;
	}
}

std::vector<VertexPosOnly>& JSONParser::getVertex(const nlohmann::json& parsedData, unsigned int index, primitiveType primType)
{
	std::vector<VertexPosOnly> countriesVertices;
    VertexPosOnly vertex;
    if (parsedData.contains("coordinates") && parsedData["coordinates"].is_array())
    {
        int polyIndex = 0;
        if (primType == primitiveType::POLYGON)
        {
            for (const auto& arr : parsedData["coordinates"])
            {
                if (arr.is_array()) {
                    for (const auto& arr2 : arr)
                    {
                        if (arr2.is_array() && arr2.size() >= 2) {
                            vertex.polygonIndex = polyIndex;
                            vertex.position = glm::vec3(arr2[0], arr2[1], 0.0f);
                            countriesVertices.push_back(vertex);
                        }
                    }
                    polyIndex++;
                }
            }
        }
        else if (primType == primitiveType::MULTIPOLYGON)
        {
            for (const auto& arr : parsedData["coordinates"])
            {
                if (arr.is_array()) {
                    for (const auto& arr2 : arr)
                    {
                        if (arr2.is_array()) {
                            for (const auto& arr3 : arr2)
                            {
                                if (arr3.is_array() && arr3.size() >= 2) {
                                    vertex.polygonIndex = polyIndex;
                                    vertex.position = glm::vec3(arr3[0], arr3[1], 0.0f);
                                    countriesVertices.push_back(vertex);
                                }
                            }
                        }
                    }
                    polyIndex++;
                }
            }
        }
        else if (primType == primitiveType::NONE)
        {
            std::cerr << "[ERROR] Primitive type is NONE!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[ERROR] Coordinates are missing or not an array!" << std::endl;
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

                size_t splitPos = tle.find("\r\n");
                if (splitPos != std::string::npos) {
                    satellite.tleLine1 = tle.substr(0, splitPos);
                    satellite.tleLine2 = tle.substr(splitPos + 2);
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


void JSONParser::ParseGeoJSON(const char* pathToGeoJSON)
{
    std::map<Country, std::pair<primitiveType, std::vector<VertexPosOnly>>> countries;
    std::ifstream GeoJSONfile(pathToGeoJSON);
	if (!GeoJSONfile.is_open())
	{
		std::cerr << "[ERROR] Failed to open GeoJSON file! Path: " << pathToGeoJSON << std::endl;
		return;
	}
    else
    {
        std::string parsedGeoData;
		std::stringstream buffer;
		buffer << GeoJSONfile.rdbuf();
		parsedGeoData = buffer.str();
        try
        {
			nlohmann::json parsedData = nlohmann::json::parse(parsedGeoData);
            if (parsedData.contains("features"))
            {
                int i = 0;
				primitiveType primitive = primitiveType::NONE;
				std::vector<VertexPosOnly> vertices;
				while (parsedData["features"][i].contains("properties"))
				{
                    Country country;
                    nlohmann::json propertiesData = parsedData["features"][i]["properties"];
                    //std::cout << "Country name: " << propertiesData["NAME"] << std::endl;
                    if (propertiesData.contains("NAME")) 
					{
						country.name = propertiesData["NAME"];
                    }
                    else
                    {
						country.name = "Unknown";
                    }
                    if (propertiesData.contains("CONTINENT")) 
					{
						country.continent = propertiesData["CONTINENT"];
                    }
                    else
                    {
						country.continent = "Unknown";
                    }

                    if (parsedData["features"][i].contains("geometry"))
                    {
						nlohmann::json geometryData = parsedData["features"][i]["geometry"];
						if (geometryData.contains("type"))
						{
							primitive = getPrimitiveType(geometryData, i);
						}
						if (geometryData.contains("coordinates"))
						{
							vertices = getVertex(geometryData, i, primitive);
						}
                    }
					countries.emplace(country, std::make_pair(primitive, vertices));
                    i++;
				}
            }
			for (auto& [key, value] : countries)
			{
				if (key.name == "Angola")
				{
					std::cout << "Country: " << key.name << " Continent: " << key.continent << std::endl;
					std::cout << "Primitive type: " << static_cast<int>(value.first) << std::endl;
					for (auto& ver : value.second)
					{
						std::cout << "Polygon index: " << ver.polygonIndex << " Position: " << ver.position.x << " " << ver.position.y << std::endl;
					}
				}
			}
		}
		catch (const nlohmann::json::exception& e)
		{
			std::cerr << "JSON parsing error: " << e.what() << std::endl;
		}
    }
}
