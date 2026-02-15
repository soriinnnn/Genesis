#ifndef GENESIS_GRAPHICS_DEVICE_H
#define GENESIS_GRAPHICS_DEVICE_H
#include <core/Base.h>
#include <core/Core.h>
#include <core/Common.h>
#include <graphics/GraphicsResource.h>
#include <d3d11.h>
#include <wrl.h>

namespace genesis 
{
    class GraphicsDevice: public Base, public std::enable_shared_from_this<GraphicsDevice> 
    {
    public:
        explicit GraphicsDevice(const GraphicsDeviceDesc& desc);
        virtual ~GraphicsDevice() override;

        SwapChainPtr createSwapChain(const SwapChainDesc& desc);
        DeviceContextPtr createDeviceContext();
        ShaderBinaryPtr compileShader(const ShaderCompileDesc& desc);
        GraphicsPipelineStatePtr createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc);
        VertexBufferPtr createVertexBuffer(const VertexBufferDesc& desc);

        void executeCommandList(DeviceContext& context);

    private:
        GraphicsResourceDesc getGraphicsResourceDesc() const noexcept;

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
        Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice;
        Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter;
        Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory;
    };
}

#endif