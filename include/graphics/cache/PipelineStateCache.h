#ifndef GENESIS_PIPELINE_STATE_CACHE_H
#define GENESIS_PIPELINE_STATE_CACHE_H
#include <graphics/cache/GraphicsCache.h>

GENESIS_NAMESPACE_START

class PipelineStateCache final: public GraphicsCache
{
public:
	explicit PipelineStateCache(const GraphicsCacheDesc& desc);
	~PipelineStateCache() override;

	SharedPtr<GraphicsPipelineState> get(const GraphicsPipelineStateDesc& desc);
	void clear() override;

private:
	SharedPtr<GraphicsPipelineState> get(size_t id);
	SharedPtr<GraphicsPipelineState> create(size_t id, const GraphicsPipelineStateDesc& desc);

private:
	HashMap<size_t, SharedPtr<GraphicsPipelineState>> m_pipelines;
};

GENESIS_NAMESPACE_END

#endif