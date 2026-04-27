#include <physics/OBPMapping.h>
#include <core/utils/Macros.h>

using namespace genesis;
using namespace JPH;

OBPMapping::OBPMapping() 
{
	m_objectToBroadPhase[ObjectLayers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
	m_objectToBroadPhase[ObjectLayers::MOVING] = BroadPhaseLayers::MOVING;
}

uint32 OBPMapping::GetNumBroadPhaseLayers() const
{
	return BroadPhaseLayers::NUM_LAYERS;
}

BroadPhaseLayer OBPMapping::GetBroadPhaseLayer(ObjectLayer layer) const
{
	GENESIS_ASSERT(layer < ObjectLayers::NUM_LAYERS, "Layer index out of bounds.");
	return m_objectToBroadPhase[layer];
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* OBPMapping::GetBroadPhaseLayerName(BroadPhaseLayer layer) const
{
	switch (static_cast<BroadPhaseLayer::Type>(layer)) {
	case (static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::NON_MOVING)):
		return "NON_MOVING";
	case (static_cast<BroadPhaseLayer::Type>(BroadPhaseLayers::MOVING)):
		return "MOVING";
	default:
		return "INVALID";
	}
}
#endif