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
			Vertex vertex;
			vertex.position = glm::vec3(x, y, z);
			vertex.normal = glm::vec3(x, y, z);
			vertex.texPos = glm::vec2(U, V);
			//std::cout << "U" << U << std::endl;
			//std::cout << "V" << V << std::endl;
			//std::cout << "texPos.x" << vertex.texPos.x << std::endl;
			//std::cout << "texPos.y" << vertex.texPos.y << std::endl;
			sphereVertices.push_back(vertex);
		}
	}

	for (int i = 0; i < sectorCount * stackCount + sectorCount; i++)
	{
		sphereIndices.push_back(i);
		sphereIndices.push_back(i + sectorCount + 1);
		sphereIndices.push_back(i + sectorCount);
		sphereIndices.push_back(i + sectorCount + 1);
		sphereIndices.push_back(i);
		sphereIndices.push_back(i + 1);
	}
}