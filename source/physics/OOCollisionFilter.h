#ifndef GENESIS_OO_COLLISION_FILTER_H
#define GENESIS_OO_COLLISION_FILTER_H
#include <physics/Layers.h>

GENESIS_NAMESPACE_START

class OOCollisionFilter: public JPH::ObjectLayerPairFilter
{
public:
	bool ShouldCollide(JPH::ObjectLayer layer1, JPH::ObjectLayer layer2) const override;
};

GENESIS_NAMESPACE_END

#endif