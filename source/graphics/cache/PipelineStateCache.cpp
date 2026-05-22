#include <graphics/cache/PipelineStateCache.h>

#define XXH_STATIC_LINKING_ONLY
#define XXH_INLINE_ALL
#include <xxhash.h>

using namespace genesis;
using namespace std;

template<>
struct hash<GraphicsPipelineStateDesc>
{
    size_t operator()(const GraphicsPipelineStateDesc& desc) const
    {
        XXH64_state_t state{};
        XXH64_reset(&state, 0);
        XXH64_update(&state, &desc.vertexShaderBinary, sizeof(desc.vertexShaderBinary));
        XXH64_update(&state, &desc.pixelShaderBinary, sizeof(desc.pixelShaderBinary));
        XXH64_update(&state, &desc.vertexShaderSignature, sizeof(desc.vertexShaderSignature));
        XXH64_update(&state, &desc.pixelShaderSignature, sizeof(desc.pixelShaderSignature));
        XXH64_update(&state, &desc.primitive, sizeof(desc.primitive));
        XXH64_update(&state, &desc.depthEnable, sizeof(desc.depthEnable));
        XXH64_update(&state, &desc.depthComparison, sizeof(desc.depthComparison));
        return static_cast<size_t>(XXH64_digest(&state));
    }
};

PipelineStateCache::PipelineStateCache(const GraphicsCacheDesc& desc): GraphicsCache(desc) {}

PipelineStateCache::~PipelineStateCache() {}

SharedPtr<GraphicsPipelineState> PipelineStateCache::get(const GraphicsPipelineStateDesc& desc)
{
    size_t id = hash<GraphicsPipelineStateDesc>{}(desc);
    SharedPtr<GraphicsPipelineState> pipeline = get(id);
    if (!pipeline) {
        pipeline = create(id, desc);
    }
	return pipeline;
}

void PipelineStateCache::clear()
{
    m_pipelines.clear();
}

SharedPtr<GraphicsPipelineState> PipelineStateCache::get(size_t id)
{
    auto it = m_pipelines.find(id);
    if (it == m_pipelines.end()) {
        return nullptr;
    }
    return it->second;
}

SharedPtr<GraphicsPipelineState> PipelineStateCache::create(size_t id, const GraphicsPipelineStateDesc& desc)
{
    auto pipeline = make_shared<GraphicsPipelineState>(desc, getGraphicsResourceDesc());
    m_pipelines.emplace(id, pipeline);
    return pipeline;
}