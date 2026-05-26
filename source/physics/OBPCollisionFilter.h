#ifndef GENESIS_OBP_COLLISION_FILTER_H
#define GENESIS_OBP_COLLISION_FILTER_H
#include <physics/Layers.h>

GENESIS_NAMESPACE_START

class OBPCollisionFilter final: public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	bool ShouldCollide(JPH::ObjectLayer layer1, JPH::BroadPhaseLayer layer2) const override;
};

GENESIS_NAMESPACE_END

#endif