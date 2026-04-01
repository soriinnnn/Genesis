#include <graphics/resources/SwapChain.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/utils/GraphicsLogUtils.h>

#define DXGI_SWAP_CHAIN_FLAGS (DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_MWA_NO_ALT_ENTER)

using namespace genesis;

static DXGI_SWAP_CHAIN_DESC createSwapChainDesc(const SwapChainDesc& desc);

SwapChain::SwapChain(const SwapChainDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{sdesc.windowSize}
{
	if (!sdesc.windowHandle) {
		GENESIS_LOG_THROW_INVALID_ARG("Window handle is null.");
	}

	DXGI_SWAP_CHAIN_DESC dxgiDesc = createSwapChainDesc(sdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_factory.CreateSwapChain(&m_device, &dxgiDesc, &m_swapChain),
		"CreateSwapChain failed."
	);
	updateRenderTargetView();

	m_depthStencil = m_graphicsDevice.createDepthStencilTexture({m_size});
}

SwapChain::~SwapChain() {}

Rect SwapChain::getSize() const noexcept
{
	return m_size;
}

void SwapChain::resize(uint32 width, uint32 height)
{
	if (width == 0 || height == 0) {
		return;
	}
	if (m_size.width() == static_cast<int32>(width) && m_size.height() == static_cast<int32>(height)) {
		return;
	}
	m_size = Rect{static_cast<int32>(width), static_cast<int32>(height)};

	m_renderTarget.Reset();
	m_depthStencil.reset();

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->ResizeBuffers(
			0, 
			width, 
			height, 
			DXGI_FORMAT_UNKNOWN, 
			DXGI_SWAP_CHAIN_FLAGS
		),
		"ResizeBuffers failed."
	);
	updateRenderTargetView();

	m_depthStencil = m_graphicsDevice.createDepthStencilTexture({m_size});
}

void SwapChain::present(bool vsync)
{
	uint32 flags = 0;
	if (!vsync) {
		flags = DXGI_PRESENT_ALLOW_TEARING;
	}

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->Present(vsync, flags),
		"Present failed."
	);
}

void SwapChain::updateRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&buffer)),
		"GetBuffer failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateRenderTargetView(buffer.Get(), nullptr, &m_renderTarget),
		"CreateRenderTargetView failed."
	);
}

/* STATIC FUNCTION DEFINITIONS */

static DXGI_SWAP_CHAIN_DESC createSwapChainDesc(const SwapChainDesc& desc) {
	DXGI_SWAP_CHAIN_DESC dxgiDesc{};
	dxgiDesc.BufferDesc.Width = desc.windowSize.width();
	dxgiDesc.BufferDesc.Height = desc.windowSize.height();
	dxgiDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiDesc.SampleDesc.Count = 1;
	dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiDesc.BufferCount = 2;
	dxgiDesc.OutputWindow = static_cast<HWND>(desc.windowHandle);
	dxgiDesc.Windowed = true;
	dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiDesc.Flags = DXGI_SWAP_CHAIN_FLAGS;

	return dxgiDesc;
}