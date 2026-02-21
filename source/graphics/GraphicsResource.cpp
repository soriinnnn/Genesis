#include <graphics/GraphicsResource.h>

using namespace genesis;

GraphicsResource::GraphicsResource(const GraphicsResourceDesc& desc): 
    Base(desc.base), m_graphicsDevice(desc.graphicsDevice), m_device(desc.device), m_factory(desc.factory) {}

GraphicsResource::~GraphicsResource() {}