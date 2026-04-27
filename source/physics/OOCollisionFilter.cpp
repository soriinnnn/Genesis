#include <physics/OOCollisionFilter.h>

using namespace genesis;
using namespace JPH;

bool OOCollisionFilter::ShouldCollide(ObjectLayer layer1, ObjectLayer layer2) const
{
    switch (layer1) {
    case ObjectLayers::NON_MOVING:
        return layer2 == ObjectLayers::MOVING;
    case ObjectLayers::MOVING:
        return true;
    default:
        return false;
    }
}