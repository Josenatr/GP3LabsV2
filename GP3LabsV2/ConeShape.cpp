#include "ConeShape.h"

ConeShape::ConeShape(float r, float h)
{
	m_radius = r;
	m_height = h;
	m_shape = new btConeShape(btScalar(r), btScalar(h));
	m_type = CShapeType::CONE;
	
}

