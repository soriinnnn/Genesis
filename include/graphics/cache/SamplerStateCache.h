#ifndef GENESIS_SAMPLER_STATE_CACHE_H
#define GENESIS_SAMPLER_STATE_CACHE_H
#include <graphics/cache/GraphicsCache.h>

namespace genesis
{
	class SamplerStateCache final: public GraphicsCache
	{
	public:
		explicit SamplerStateCache(const GraphicsCacheDesc& desc);
		~SamplerStateCache() override;

		SharedPtr<SamplerState> get(const SamplerStateDesc& desc);
		void clear() override;

	private:
		SharedPtr<SamplerState> get(size_t id);
		SharedPtr<SamplerState> create(size_t id, const SamplerStateDesc& desc);

	private:
		HashMap<size_t, SharedPtr<SamplerState>> m_samplers;
	};
}

#endif
