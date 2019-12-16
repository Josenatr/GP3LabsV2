#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CylinderShape : public CollisionShape
{
public:
	CylinderShape(glm::vec3 halfExtents);

private:
	glm::vec3 m_halfExtents;
};