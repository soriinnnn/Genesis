#ifndef GENESIS_GRAPHICS_RESOURCE_H
#define GENESIS_GRAPHICS_RESOURCE_H
#include <core/Base.h>
#include <d3d11.h>
#include <wrl.h>

namespace genesis
{
	struct GraphicsResourceDesc
	{
		BaseDesc base;
		std::shared_ptr<const GraphicsDevice> graphicsDevice;
		ID3D11Device& device;
		IDXGIFactory& factory;
	};

	class GraphicsResource: public Base
	{
	public:
		explicit GraphicsResource(const GraphicsResourceDesc& desc);
		virtual ~GraphicsResource() override;

	protected:
		std::shared_ptr<const GraphicsDevice> m_graphicsDevice;
		ID3D11Device& m_device;
		IDXGIFactory& m_factory;
	};
}

#endif