#ifndef GENESIS_LAYERS_H
#define GENESIS_LAYERS_H
#include <core/Core.h>
#include <jolt/Jolt.h>
#include <jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

GENESIS_NAMESPACE_BEGIN

namespace ObjectLayers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr uint32 NUM_LAYERS = 2;
}

namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING{0};
	static constexpr JPH::BroadPhaseLayer MOVING{1};
	static constexpr uint32 NUM_LAYERS = 2;
}

GENESIS_NAMESPACE_END

#endif