#include <math/Vec3.h>

using namespace genesis;

// --------------------------------------------------------------------------------

Vec3::Vec3()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

const float* Vec3::toArray() const noexcept
{
    return &m_x;
}
