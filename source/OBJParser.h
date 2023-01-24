//----------------------------------------------------------------------------------------
/**
 * \file       OBJParser.cpp
 * \author     Bogdan Putintsev
 * \date       2021/05/13
 * \brief      Simple parser for OBJ files.
 *
*/
//----------------------------------------------------------------------------------------


#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <pgr.h>

/// <summary>
/// Open the OBJ file and read all the information about vertices and push it to the vector of vertices.
/// </summary>
/// <param name="path">Path to the object</param>
/// <param name="returnVector">Reference to the vector with the future vertices</param>
/// <returns>bool</returns>
bool readOBJ(const char* path, std::vector<float>& returnVector)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> returnVertices;
	std::vector<glm::vec2> returnUvs;
	std::vector<glm::vec3> returnNormals;
	std::vector<unsigned int> vertexIndexes;
	std::vector<unsigned int> uvIndexes;
	std::vector<unsigned int> normalIndexes;

	FILE* file = fopen(path, "r");
	if (file == NULL)
		return false;

	char line[256];

	while (fscanf(file, "%s", line) != EOF)
	{
		if (strcmp(line, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(line, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(line, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			vertexIndexes.push_back(vertexIndex[0]);
			vertexIndexes.push_back(vertexIndex[1]);
			vertexIndexes.push_back(vertexIndex[2]);
			uvIndexes.push_back(uvIndex[0]);
			uvIndexes.push_back(uvIndex[1]);
			uvIndexes.push_back(uvIndex[2]);
			normalIndexes.push_back(normalIndex[0]);
			normalIndexes.push_back(normalIndex[1]);
			normalIndexes.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndexes.size(); i++)
	{
		glm::vec3 vertex = vertices[vertexIndexes[i] - 1];
		glm::vec2 texture = uvs[uvIndexes[i] - 1];
		glm::vec3 normal = normals[normalIndexes[i] - 1];
		returnVertices.push_back(vertex);
		returnUvs.push_back(texture);
		returnNormals.push_back(normal);
	}

	for (unsigned int i = 0; i < returnVertices.size(); ++i)
	{
		returnVector.push_back(returnVertices[i].x);
		returnVector.push_back(returnVertices[i].y);
		returnVector.push_back(returnVertices[i].z);
		returnVector.push_back(returnUvs[i].x);
		returnVector.push_back(returnUvs[i].y);
		returnVector.push_back(returnNormals[i].x);
		returnVector.push_back(returnNormals[i].y);
		returnVector.push_back(returnNormals[i].z);
	}

	return true;
}
