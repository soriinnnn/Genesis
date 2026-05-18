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
		const char* path;
	};

	class FontAtlas final: GraphicsResource
	{
	public:
		struct Glyph
		{
			uint32 character;
			Rect subRect;
			float xOffset;
			float yOffset;
			float xAdvance;
		};

	public:
		FontAtlas(const FontAtlasDesc& fdesc, const GraphicsResourceDesc& gdesc);
		~FontAtlas() override;

		bool contains(wchar_t character) const;

		Rect getSize(const wchar_t* text) const;
		Glyph getGlyph(wchar_t character) const;
		float getLineSpacing() const;

		void setPixelAlignment(bool enable);
		void setLineSpacing(float spacing);
		void setDefaultCharacter(wchar_t character);

	private:
		UniquePtr<DirectX::DX11::SpriteFont> m_font;

		friend class SpriteBatch;
	};
}

#endif