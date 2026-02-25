#ifndef GENESIS_GRAPHICS_DEVICE_H
#define GENESIS_GRAPHICS_DEVICE_H
#include <core/Base.h>
#include <core/Core.h>
#include <graphics/GraphicsResource.h>
#include <graphics/SwapChain.h>
#include <graphics/ShaderBinary.h>
#include <graphics/GraphicsPipelineState.h>
#include <graphics/VertexBuffer.h>
#include <graphics/VertexShaderSignature.h>
#include <graphics/ConstantBuffer.h>
#include <d3d11.h>
#include <wrl.h>

namespace genesis 
{
    struct GraphicsDeviceDesc
    {
        BaseDesc base;
    };

    class GraphicsDevice: public Base, public std::enable_shared_from_this<GraphicsDevice> 
    {
    public:
        explicit GraphicsDevice(const GraphicsDeviceDesc& desc);
        virtual ~GraphicsDevice() override;

        SharedPtr<SwapChain> createSwapChain(const SwapChainDesc& desc);
        SharedPtr<DeviceContext> createDeviceContext();
        SharedPtr<ShaderBinary> compileShader(const ShaderCompileDesc& desc);
        SharedPtr<GraphicsPipelineState> createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc);
        SharedPtr<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc);
        SharedPtr<VertexShaderSignature> createVertexShaderSignature(const VertexShaderSignatureDesc& desc);
        SharedPtr<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc);

        void clearState();
        void executeCommandList(DeviceContext& context);

    private:
        GraphicsResourceDesc getGraphicsResourceDesc() noexcept;

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
        Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice;
        Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter;
        Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory;
    };
}

#endif