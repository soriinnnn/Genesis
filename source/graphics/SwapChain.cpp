#include <graphics/SwapChain.h>
#include <graphics/GraphicsLogUtils.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

static DXGI_SWAP_CHAIN_DESC createSwapChainDesc(const SwapChainDesc& desc);

SwapChain::SwapChain(const SwapChainDesc& scDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (scDesc.wndSize.width() == 0 || scDesc.wndSize.height() == 0) {
		GENESIS_LOG_THROW_INVALID_ARG("Window size can't be zero.");
	}
	if (!scDesc.wndHandle) {
		GENESIS_LOG_THROW_INVALID_ARG("Window handle is null.");
	}

	DXGI_SWAP_CHAIN_DESC dxgiDesc = createSwapChainDesc(scDesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_factory.CreateSwapChain(&m_device, &dxgiDesc, &m_swapChain),
		"CreateSwapChain failed."
	);
	updateRenderTargetView();
	m_size = scDesc.wndSize;
}

SwapChain::~SwapChain() {}

Rect SwapChain::getSize() const noexcept
{
	return m_size;
}

void SwapChain::resize(uint32 width, uint32 height)
{
	m_size = Rect{width, height};

	m_graphicsDevice->clearState();
	m_renderTarget.Reset();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->ResizeBuffers(
			0, 
			width, 
			height, 
			DXGI_FORMAT_UNKNOWN, 
			0
		),
		"ResizeBuffers failed."
	);
	updateRenderTargetView();
}

void SwapChain::present(bool vsync)
{
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->Present(vsync, 0),
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
	dxgiDesc.BufferDesc.Width = desc.wndSize.width();
	dxgiDesc.BufferDesc.Height = desc.wndSize.height();
	dxgiDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiDesc.SampleDesc.Count = 1;
	dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiDesc.BufferCount = 2;
	dxgiDesc.OutputWindow = static_cast<HWND>(desc.wndHandle);
	dxgiDesc.Windowed = true;
	dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	return dxgiDesc;
}