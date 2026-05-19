#include <resources/Font.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Font::Font(const FontDesc& desc): Resource(desc.resource)
{
	m_font = desc.resource.graphicsContext.graphicsDevice.createFontAtlas({desc.resource.path});
}

Font::~Font() {}

const FontAtlas& Font::getFontAtlas() const
{
	return *m_font;
}
