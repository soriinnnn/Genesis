#include <graphics/resources/SpriteBatch.h>
#include <graphics/resources/FontAtlas.h>
#include <graphics/resources/DeviceContext.h>

using namespace genesis;
using namespace std;

SpriteBatch::SpriteBatch(const SpriteBatchDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	try {
		m_batch = make_unique<DirectX::DX11::SpriteBatch>(sdesc.deviceContext.m_context.Get());
	}
	catch (const std::exception& e) {
		GENESIS_LOG_THROW_ERROR("SpriteBatch creation failed.\nDetails: {}", e.what());
	}
}

SpriteBatch::~SpriteBatch() {}

void SpriteBatch::drawText(FontAtlas& font, const char* text, Point pos, Vec4 color)
{
	DirectX::XMFLOAT4 dxColor = DirectX::XMFLOAT4(color.toArray());
	font.m_font->DrawString(
		m_batch.get(), 
		text, 
		DirectX::XMFLOAT2(static_cast<float>(pos.x), static_cast<float>(pos.y)),
		DirectX::XMLoadFloat4(&dxColor)
	);
}