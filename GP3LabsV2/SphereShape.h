#pragma once
#include "pch.h"
#include "CollisionShape.h"

class SphereShape : public CollisionShape
{
public:
	SphereShape(float s);

private:
	float m_scalar;
};