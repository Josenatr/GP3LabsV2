#pragma once
#include "pch.h"
#include "Vertex.h"
namespace Quad
{
	std::vector<Vertex> quadVertices
	{
	Vertex(glm::vec3(0.5,0.5f,0.f), glm::vec4(1), glm::vec2(1.0f,1.0f)), //topRight
	Vertex(glm::vec3(0.5f,-0.5f,0.f),glm::vec4(1), glm::vec2(1.0f,0.0f)), //bottomRight
	Vertex(glm::vec3(-0.5f,-0.5f,0),glm::vec4(1), glm::vec2(0.0f,0.0f)), //bottomLeft
	Vertex(glm::vec3(-0.5f,0.5f,0), glm::vec4(1), glm::vec2(0.0f,1.0f)), //topLeft
	};
	std::vector<int> quadIndices
	{
	0, 1, 3,
	1, 2, 3
	};
}
