#include <graphics/GraphicsCache.h>

using namespace genesis;

GraphicsCache::GraphicsCache(const GraphicsCacheDesc& desc): Base(desc.base), m_graphicsDevice{desc.graphicsDevice} {}

GraphicsResourceDesc GraphicsCache::getGraphicsResourceDesc()
{
    return m_graphicsDevice.getGraphicsResourceDesc();
}

GraphicsCache::~GraphicsCache() {}