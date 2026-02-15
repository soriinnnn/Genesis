#include <math/Vec4.h>

using namespace genesis;

// --------------------------------------------------------------------------------

Vec4::Vec4() 
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 0;
}

Vec4::Vec4(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

const float* Vec4::toArray() const noexcept
{
	return &m_x;
}
