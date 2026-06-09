#ifndef GENESIS_PHYSICS_TYPES_H
#define GENESIS_PHYSICS_TYPES_H
#include <core/Core.h>
#include <math/Vec3.h>

GENESIS_NAMESPACE_BEGIN

enum class MotionType
{
	Static,
	Kinematic,
	Dynamic
};

struct BodySettings
{
	Vec3 position;
	MotionType motionType = MotionType::Static;
	float mass = 1.0f;
};

GENESIS_NAMESPACE_END

#endif