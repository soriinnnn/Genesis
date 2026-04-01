#include <graphics/SamplerStateCache.h>

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

using namespace genesis;
using namespace std;

template<>
struct hash<SamplerStateDesc>
{
	size_t operator()(const SamplerStateDesc& desc) const
	{
		XXH64_state_t state{};
		XXH64_reset(&state, 0);
		XXH64_update(&state, &desc.filter, sizeof(desc.filter));
		XXH64_update(&state, &desc.addressU, sizeof(desc.addressU));
		XXH64_update(&state, &desc.addressV, sizeof(desc.addressV));
		XXH64_update(&state, &desc.addressW, sizeof(desc.addressW));
		XXH64_update(&state, &desc.comparison, sizeof(desc.comparison));
		XXH64_update(&state, &desc.maxAnisotropy, sizeof(desc.maxAnisotropy));
		XXH64_update(&state, &desc.minLOD, sizeof(desc.minLOD));
		XXH64_update(&state, &desc.maxLOD, sizeof(desc.maxLOD));
		return static_cast<size_t>(XXH64_digest(&state));
	}
};

SamplerStateCache::SamplerStateCache(const GraphicsCacheDesc& desc): GraphicsCache(desc) {}

SamplerStateCache::~SamplerStateCache() {}

SharedPtr<SamplerState> SamplerStateCache::get(const SamplerStateDesc& desc)
{
	size_t id = hash<SamplerStateDesc>{}(desc);
	SharedPtr<SamplerState> sampler = get(id);
	if (!sampler) {
		sampler = create(id, desc);
	}
	return sampler;
}

void SamplerStateCache::clear()
{
	m_samplers.clear();
}

SharedPtr<SamplerState> SamplerStateCache::get(size_t id)
{
	auto it = m_samplers.find(id);
	if (it == m_samplers.end()) {
		return nullptr;
	}
	return it->second;
}

SharedPtr<SamplerState> SamplerStateCache::create(size_t id, const SamplerStateDesc& desc)
{
	auto sampler = make_shared<SamplerState>(desc, getGraphicsResourceDesc());
	m_samplers.emplace(id, sampler);
	return sampler;
}