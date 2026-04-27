#ifndef GENESIS_OBP_COLLISION_FILTER_H
#define GENESIS_OBP_COLLISION_FILTER_H
#include <physics/Layers.h>

namespace genesis
{
	class OBPCollisionFilter final: public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		bool ShouldCollide(JPH::ObjectLayer layer1, JPH::BroadPhaseLayer layer2) const override;
	};
}

#endif