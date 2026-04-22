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

void SpriteBatch::begin(SamplerState* samplerState)
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

void SpriteBatch::end()
{
	m_batch->End();
}

void SpriteBatch::drawText(FontAtlas& font, const char* text, Point pos, Vec2 scale, Vec4 color)
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
		dxScale
	);
}

void SpriteBatch::drawImage(ImageTexture* image, Point pos, Vec2 scale, Vec4 color)
{
	ID3D11ShaderResourceView* resourceView = (image != nullptr) ? image->m_resourceView.Get() : m_whiteTexture->m_resourceView.Get();
	DirectX::XMFLOAT2 dxPosition = DirectX::XMFLOAT2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	DirectX::XMFLOAT2 dxScale = DirectX::XMFLOAT2(scale.toArray());
	DirectX::XMFLOAT4 dxColor = DirectX::XMFLOAT4(color.toArray());

	m_batch->Draw(
		resourceView,
		dxPosition,
		nullptr,
		DirectX::XMLoadFloat4(&dxColor),
		0.0f,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		dxScale
	);
}