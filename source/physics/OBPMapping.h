#ifndef GENESIS_OBP_MAPPING_H
#define GENESIS_OBP_MAPPING_H
#include <physics/Layers.h>

GENESIS_NAMESPACE_BEGIN

class OBPMapping final: public JPH::BroadPhaseLayerInterface
{
public:
	OBPMapping();

	uint32 GetNumBroadPhaseLayers() const override;
	JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override;
#endif

private:
	JPH::BroadPhaseLayer m_objectToBroadPhase[ObjectLayers::NUM_LAYERS];
};

GENESIS_NAMESPACE_END

#endif