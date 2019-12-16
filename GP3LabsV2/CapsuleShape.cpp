#include "CapsuleShape.h"

CapsuleShape::CapsuleShape(float r, float h)
{
	m_radius = r;
	m_height = h;
	m_shape = new btCapsuleShape(r, h);
	m_type = CShapeType::CAPSULE;
}
