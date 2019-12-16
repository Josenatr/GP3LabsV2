#pragma once
#include "pch.h"
#include "CollisionShape.h"

class CapsuleShape : public CollisionShape
{
public:
	CapsuleShape(float r, float h);
	
private:
	float m_radius;
	float m_height;
};

