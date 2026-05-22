#ifndef GENESIS_VEC4_H
#define GENESIS_VEC4_H
#include <math/Vec3.h>

namespace genesis 
{
	class Vec4
	{
	public:
		Vec4();
		Vec4(float x, float y, float z);
		Vec4(float x, float y, float z, float w);
		explicit Vec4(const Vec3& v, float w = 1.0f);

		const float* toArray() const noexcept;
		bool operator==(const Vec4& rhs) const noexcept;

	public:
		float x, y, z, w;
	};
}

#endif