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
	unsigned int polygonIndex;
	glm::vec3 position;
	VertexPosOnly() : polygonIndex(0), position(glm::vec3(0.0f)) {}
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

class JSONParser
{
private:
	primitiveType getPrimitiveType(const nlohmann::json& parsedData, unsigned int index);
	std::vector<VertexPosOnly>& getVertex(const nlohmann::json& parsedData, unsigned int index, primitiveType primType);
public:
	JSONParser();
	~JSONParser();
	static void ParseJSONSattelite(const std::string& satData, std::vector<Satellite>& satellites);
    void ParseGeoJSON(const char* pathToGeoJSON);
};
#endif