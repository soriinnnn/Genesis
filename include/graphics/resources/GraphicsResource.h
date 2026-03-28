#ifndef GENESIS_GRAPHICS_RESOURCE_H
#define GENESIS_GRAPHICS_RESOURCE_H
#include <core/Base.h>
#include <core/Core.h>
#include <d3d11.h>
#include <wrl.h>

namespace genesis
{
	struct GraphicsResourceDesc
	{
		BaseDesc base;
		GraphicsDevice& graphicsDevice;
		ID3D11Device& device;
		IDXGIFactory& factory;
	};

	class GraphicsResource: public Base
	{
	public:
		virtual ~GraphicsResource() override;

	protected:
		explicit GraphicsResource(const GraphicsResourceDesc& desc);

	protected:
		GraphicsDevice& m_graphicsDevice;
		ID3D11Device& m_device;
		IDXGIFactory& m_factory;
	};
}

#endif