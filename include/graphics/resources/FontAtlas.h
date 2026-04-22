#ifndef GENESIS_FONT_ATLAS_H
#define GENESIS_FONT_ATLAS_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>
#include <math/Point.h>
#include <SpriteFont.h>

namespace genesis
{
	struct FontAtlasDesc
	{
		const wchar_t* path;
	};

	class FontAtlas final: GraphicsResource
	{
	public:
		FontAtlas(const FontAtlasDesc& fdesc, const GraphicsResourceDesc& gdesc);
		~FontAtlas() override;

		Rect getSize(const char* text);

	private:
		UniquePtr<DirectX::DX11::SpriteFont> m_font;

		friend class SpriteBatch;
	};
}

#endif