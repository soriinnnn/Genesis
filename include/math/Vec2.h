#ifndef GENESIS_VEC2_H
#define GENESIS_VEC2_H

namespace genesis
{
	class Vec2
	{
	public:
		Vec2();
		Vec2(float x, float y);

		const float* toArray() const noexcept;

	public:
		float x, y;
	};
}

#endif