#ifndef GENESIS_GRAPHICS_CACHE_H
#define GENESIS_GRAPHICS_CACHE_H
#include <core/Base.h>
#include <core/Core.h>
#include <graphics/GraphicsDevice.h>

namespace genesis
{
	struct GraphicsCacheDesc
	{
		BaseDesc base;
		GraphicsDevice& graphicsDevice;
	};

	class GraphicsCache: public Base
	{
	public:
		virtual ~GraphicsCache() override;
		virtual void clear() = 0;

	protected:
		explicit GraphicsCache(const GraphicsCacheDesc& desc);
		GraphicsResourceDesc getGraphicsResourceDesc();

	private:
		GraphicsDevice& m_graphicsDevice;
	};
}

#endif