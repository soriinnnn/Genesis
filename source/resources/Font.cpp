#include <resources/Font.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Font::Font(const FontDesc& desc): Resource(desc.resource)
{
	wstring path{m_path.begin(), m_path.end()};
	m_font = desc.resource.graphicsContext.graphicsDevice.createFontAtlas({path.c_str()});
}

Font::~Font() {}

FontAtlas& Font::getFontAtlas()
{
	return *m_font;
}
