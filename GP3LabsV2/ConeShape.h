#pragma once
#include "pch.h"
#include "CollisionShape.h"
class ConeShape : public CollisionShape
{
public:
	ConeShape(float r, float h);

private:
	float m_radius;
	float m_height;
};

