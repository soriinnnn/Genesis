#include <physics/OBPCollisionFilter.h>

using namespace genesis;
using namespace JPH;

bool OBPCollisionFilter::ShouldCollide(JPH::ObjectLayer layer1, JPH::BroadPhaseLayer layer2) const
{
    switch (layer1) {
    case ObjectLayers::NON_MOVING:
        return layer2 == BroadPhaseLayers::MOVING;
    case ObjectLayers::MOVING:
        return true;
    default:
        return false;
    }
}
