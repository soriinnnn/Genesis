#include <graphics/resources/SpriteBatch.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/FontAtlas.h>
#include <graphics/resources/DeviceContext.h>
#include <graphics/resources/SamplerState.h>
#include <graphics/resources/ImageTexture.h>

using namespace genesis;
using namespace std;

SpriteBatch::SpriteBatch(const SpriteBatchDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	try {
		m_batch = make_unique<DirectX::DX11::SpriteBatch>(sdesc.deviceContext.m_context.Get());
		m_states = make_unique<DirectX::CommonStates>(&m_device);
		m_whiteTexture = m_graphicsDevice->createImageTexture(ImageTextureSolidDesc{});
	}
	catch (const std::exception& e) {
		GENESIS_LOG_THROW_ERROR("SpriteBatch creation failed.\nDetails: {}", e.what());
	}
}

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::begin(const SamplerState* samplerState)
{
	ID3D11SamplerState* dxSamplerState = samplerState ? samplerState->m_samplerState.Get() : nullptr;

	m_batch->Begin(
		DirectX::DX11::SpriteSortMode_Deferred,
		nullptr,
		dxSamplerState,
		nullptr,
		nullptr,
		nullptr,
		DirectX::XMMatrixIdentity()
	);
}

void SpriteBatch::drawText(const FontAtlas& font, const wchar_t* text, const Point& pos, const Vec2& scale, const Vec4& color)
{
	DirectX::XMFLOAT2 dxPosition = DirectX::XMFLOAT2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	DirectX::XMFLOAT2 dxScale = DirectX::XMFLOAT2(scale.toArray());
	DirectX::XMFLOAT4 dxColor = DirectX::XMFLOAT4(color.toArray());
	
	font.m_font->DrawString(
		m_batch.get(), 
		text, 
		dxPosition,
		DirectX::XMLoadFloat4(&dxColor),
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		dxScale,
		DirectX::DX11::SpriteEffects_None,
		0.0f
	);
}

void SpriteBatch::drawImage(const ImageTexture& image, const Rect& sourceRect, const Point& pos, const Vec2& scale, const Vec4& color)
{
	ID3D11ShaderResourceView* resourceView = image.m_resourceView.Get();
	RECT dxSourceRect = RECT{static_cast<LONG>(sourceRect.left), static_cast<LONG>(sourceRect.top), static_cast<LONG>(sourceRect.right), static_cast<LONG>(sourceRect.bottom)};
	DirectX::XMFLOAT2 dxPosition = DirectX::XMFLOAT2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	DirectX::XMFLOAT2 dxScale = DirectX::XMFLOAT2(scale.toArray());
	DirectX::XMFLOAT4 dxColor = DirectX::XMFLOAT4(color.toArray());

	m_batch->Draw(
		resourceView,
		dxPosition,
		&dxSourceRect,
		DirectX::XMLoadFloat4(&dxColor),
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		dxScale,
		DirectX::DX11::SpriteEffects_None,
		0.0f
	);
}

void SpriteBatch::drawSolid(const Rect& size, const Point& pos, const Vec2& scale, const Vec4& color)
{
	ID3D11ShaderResourceView* resourceView = m_whiteTexture->m_resourceView.Get();
	DirectX::XMFLOAT2 dxPosition = DirectX::XMFLOAT2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	DirectX::XMFLOAT2 dxScale = DirectX::XMFLOAT2(size.width() * scale.x, size.height() * scale.y);
	DirectX::XMFLOAT4 dxColor = DirectX::XMFLOAT4(color.toArray());

	m_batch->Draw(
		resourceView,
		dxPosition,
		nullptr,
		DirectX::XMLoadFloat4(&dxColor),
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		dxScale,
		DirectX::DX11::SpriteEffects_None,
		0.0f
	);
}

void SpriteBatch::end()
{
	m_batch->End();
}