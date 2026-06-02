#include <graphics/resources/SwapChain.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/utils/GraphicsLogUtils.h>

#define DEFAULT_BUFFER_COUNT 2
#define DEFAULT_FLAGS DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING

using namespace genesis;

static DXGI_SWAP_CHAIN_DESC getDXGISwapChainDesc(void* handle, const Rect& size);

SwapChain::SwapChain(const SwapChainDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{sdesc.size}
{
	if (!sdesc.wndHandle) {
		GENESIS_LOG_THROW_INVALID_ARG("Window handle is null.");
	}

	DXGI_SWAP_CHAIN_DESC desc = getDXGISwapChainDesc(sdesc.wndHandle, sdesc.size);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_factory.CreateSwapChain(&m_device, &desc, &m_swapChain),
		"CreateSwapChain failed."
	);
	updateBuffers();
	
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_factory.MakeWindowAssociation(
			static_cast<HWND>(sdesc.wndHandle), 
			DXGI_MWA_NO_WINDOW_CHANGES
		),
		"MakeWindowAssociation failed."
	);
}

SwapChain::~SwapChain() 
{
	if (m_swapChain) {
		m_swapChain->SetFullscreenState(FALSE, nullptr);
	}
}

bool SwapChain::isFullscreen() const
{
	BOOL isFullscreen;
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->GetFullscreenState(&isFullscreen, nullptr),
		"GetFullscreenState failed."
	);
	return static_cast<bool>(isFullscreen);
}

Rect SwapChain::getSize() const noexcept
{
	return m_size;
}

bool SwapChain::setFullscreen(bool enable)
{
	HRESULT result = m_swapChain->SetFullscreenState(static_cast<BOOL>(enable), nullptr);
	switch (result) {
		case S_OK: {
			updateState();
			return true;
		}
		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
			GENESIS_LOG_WARNING("Cannot change fullscreen state. The operation is not currently available.");
			return false;
		case DXGI_STATUS_MODE_CHANGE_IN_PROGRESS:
			GENESIS_LOG_WARNING("A fullscreen mode change is already in progress.");
			return false;
		default:
			GENESIS_LOG_THROW_ERROR("SetFullscreenState failed.\nError code: 0x{:08X}", static_cast<uint32>(result));
	}
}

void SwapChain::setSize(const Rect& size)
{
	if (m_size == size) {
		return;
	}
	if (size.width() == 0 || size.height() == 0) {
		GENESIS_LOG_WARNING("Cannot set swap chain size to zero. Ignoring.");
		return;
	}
	m_size = size;

	m_targetView.Reset();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->ResizeBuffers(
			0, 
			static_cast<uint32>(size.width()),
			static_cast<uint32>(size.height()),
			DXGI_FORMAT_UNKNOWN, 
			DEFAULT_FLAGS
		),
		"ResizeBuffers failed."
	);
	updateBuffers();
}

void SwapChain::updateState()
{
	m_targetView.Reset();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->ResizeBuffers(
			0,
			static_cast<uint32>(m_size.width()),
			static_cast<uint32>(m_size.height()),
			DXGI_FORMAT_UNKNOWN,
			DEFAULT_FLAGS
		),
		"ResizeBuffers failed."
	);
	updateBuffers();
}

void SwapChain::present(bool vsync)
{
	uint32 flags = 0;
	if (!vsync && !isFullscreen()) {
		flags = DXGI_PRESENT_ALLOW_TEARING;
	}
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->Present(vsync, flags),
		"Present failed."
	);
}

void SwapChain::updateBuffers()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&buffer)),
		"GetBuffer failed."
	);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateRenderTargetView(buffer.Get(), nullptr, &m_targetView),
		"CreateRenderTargetView failed."
	);
	m_depthStencil = m_graphicsDevice->createDepthStencilTexture({m_size});
}

/* STATIC FUNCTION DEFINITIONS */

DXGI_SWAP_CHAIN_DESC getDXGISwapChainDesc(void* handle, const Rect& size) {
	DXGI_SWAP_CHAIN_DESC desc{};

	desc.BufferDesc.Width = static_cast<UINT>(size.width());
	desc.BufferDesc.Height = static_cast<UINT>(size.height());
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = DEFAULT_BUFFER_COUNT;
	desc.OutputWindow = static_cast<HWND>(handle);
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags = DEFAULT_FLAGS;

	return desc;
}