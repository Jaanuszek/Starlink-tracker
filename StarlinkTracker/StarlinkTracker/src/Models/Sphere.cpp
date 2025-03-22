#include "../../include/Models/Sphere.h"

Sphere::Sphere(int sectorCount, int stackCount, float radius)
{
	CreateSphere(sectorCount, stackCount, radius);
}

Sphere::~Sphere()
{

}

void Sphere::CreateSphere(int sectorCount, int stackCount, float radius) 
{
	const float PI = 3.1415926f;
	sphereVertices.resize((sectorCount + 1) * (stackCount + 1));
	sphereIndices.resize(sectorCount * stackCount * 6);

	//Vertices and texture coordinates
	std::vector<Vertex>::iterator vertexIterator = sphereVertices.begin();
	for (int i = 0; i <= stackCount; i++)
	{
		float V = (float)i / (float)stackCount;
		float phi = V * PI;

		for (int j = 0; j <= sectorCount; j++)
		{
			float U = (float)j / (float)sectorCount;
			float theta = U * (PI * 2);
			float x = cos(theta) * sin(phi) * radius;
			float y = cos(phi) * radius;
			float z = sin(theta) * sin(phi) * radius;
			vertexIterator->position = glm::vec3(x, y, z);
			vertexIterator->normal = glm::vec3(x, y, z);
			vertexIterator->texPos = glm::vec2(U, V);
			++vertexIterator;
		}
	}

	// Indices
	std::vector<unsigned int>::iterator indicesIterator = sphereIndices.begin();
	for (int i = 0; i < stackCount; i++)
	{
		for (int j = 0; j < sectorCount; j++)
		{
			int first = (i * (sectorCount + 1)) + j;
			int second = first + sectorCount + 1;

			*indicesIterator++ = first;
			*indicesIterator++ = second;
			*indicesIterator++ = first + 1;
			*indicesIterator++ = second;
			*indicesIterator++ = second + 1;
			*indicesIterator++ = first + 1;
		}
	}
}