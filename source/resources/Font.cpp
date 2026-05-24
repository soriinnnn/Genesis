#include <resources/Font.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Font::Font(const FontDesc& desc): Resource(desc.resource)
{
	m_atlas = desc.resource.graphicsContext.graphicsDevice.createFontAtlas({desc.resource.path});
}

Font::~Font() {}

FontAtlas& Font::getAtlas()
{
	return *m_atlas;
}
