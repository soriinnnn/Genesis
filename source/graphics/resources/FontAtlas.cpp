#include <graphics/resources/FontAtlas.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;
using namespace std;
using namespace DirectX;
using namespace DirectX::DX11;

FontAtlas::FontAtlas(const FontAtlasDesc& fdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	try {
		m_font = make_unique<SpriteFont>(&m_device, fdesc.path);
	}
	catch (const std::exception& e) {
		GENESIS_LOG_THROW_ERROR("FontAtlas creation failed.\nDetails: {}", e.what());
	}
}

FontAtlas::~FontAtlas() {}

Rect FontAtlas::getSize(const char* text)
{
	RECT bounds = m_font->MeasureDrawBounds(text, DirectX::XMFLOAT2(0.0f, 0.0f));
	return Rect{
		bounds.left + bounds.right,
		bounds.top + bounds.bottom
	};
}