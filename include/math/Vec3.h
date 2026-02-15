#ifndef GENESIS_VEC3_H
#define GENESIS_VEC3_H

namespace genesis
{
	class Vec3
	{
	public:
		Vec3();
		Vec3(float x, float y, float z);

		const float* toArray() const noexcept;

	private:
		float m_x, m_y, m_z;
	};
}

#endif