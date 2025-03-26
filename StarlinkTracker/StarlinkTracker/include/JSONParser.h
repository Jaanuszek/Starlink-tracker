#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <iostream>
#include <json.hpp>
#include <vector>
#include <map>
#include <fstream>
#include <functional>
#include "SGP4.h"
#include "glm/glm.hpp"
#include "VBO.h" // to have access to Vertex struct

#define M_PI 3.14159265358979323846

struct Satellite {
    int satid;
    std::string satname;
    int transactionscount;
    std::string tleLine1;
    std::string tleLine2;
};

struct Country {
    std::string name;
    std::string continent;

	bool operator<(const Country& other) const {
		if (name != other.name)
		{
			return name < other.name;
		}
		return continent < other.continent;
	}
};

struct VertexPosOnly {
	glm::vec3 position;
};

enum class primitiveType {
	POINT,
	LINESTRING,
	POLYGON,
	MULTIPOINT,
	MULTILINESTRING,
	MULTIPOLYGON,
	GEOMETRYCOLLECTION,
	NONE
};

struct PrimitiveData{
	primitiveType type;
	std::map<unsigned int, std::vector<VertexPosOnly>> polygons;
};

class JSONParser
{
private:
	std::map<Country, PrimitiveData> countriesMap;
	primitiveType getPrimitiveType(const nlohmann::json& parsedData, unsigned int index);
	glm::vec3 changeCoordsToSphere(float lon, float lat, float radius);
	std::map<unsigned int, std::vector<VertexPosOnly>> getVertex(const nlohmann::json& parsedData, unsigned int index, primitiveType primType);
public:
	JSONParser();
	~JSONParser();
	static void ParseJSONSattelite(const std::string& satData, std::vector<Satellite>& satellites);
    void ParseGeoJSON(const char* pathToGeoJSON);
	const std::map<Country, PrimitiveData>& getCountries() { return countriesMap; }
};
#endif