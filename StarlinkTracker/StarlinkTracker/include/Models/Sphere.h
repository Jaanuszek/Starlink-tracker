#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <vector>
#include "../VBO.h"

class Sphere
{
private:
	std::vector<Vertex> sphereVertices;
	std::vector<unsigned int> sphereIndices;
public:
	Sphere(int sectorCount, int stackCount, float radius);
	~Sphere();
	void CreateSphere(int sectorCount, int stackCount, float radius);
	const std::vector<Vertex>& getVertices() const { return sphereVertices; }
	const std::vector<unsigned int>& getIndices() const { return sphereIndices; }
};


#endif 