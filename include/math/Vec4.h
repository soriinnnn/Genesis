
#ifndef GENESIS_VEC4_H
#define GENESIS_VEC4_H

namespace genesis 
{
	class Vec4
	{
	public:
		Vec4();
		Vec4(float x, float y, float z, float w);

		const float* toArray() const noexcept;

	private:
		float m_x, m_y, m_z, m_w;
	};
}

#endif