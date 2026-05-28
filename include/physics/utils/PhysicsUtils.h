#ifndef GENESIS_PHYSICS_UTILS_H
#define GENESIS_PHYSICS_UTILS_H
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

namespace physicsUtils
{
	constexpr float FIXED_UPDATE_HZ = 60.0f;
	constexpr float FIXED_UPDATE_DT = 1.0f / FIXED_UPDATE_HZ;
	constexpr int MAX_UPDATE_STEPS = 5;

	inline int getUpdateSteps(float& accumulator)
	{
		int steps = 0;

		while (accumulator >= FIXED_UPDATE_DT && steps < MAX_UPDATE_STEPS) {
			steps += 1;
			accumulator -= FIXED_UPDATE_DT;
		}
		if (steps == MAX_UPDATE_STEPS) {
			accumulator = 0.0f;
		}

		return steps;
	}
}

GENESIS_NAMESPACE_END

#endif