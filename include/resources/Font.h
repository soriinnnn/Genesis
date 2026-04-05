#ifndef GENESIS_FONT_H
#define GENESIS_FONT_H
#include <resources/Resource.h>

namespace genesis
{
	struct FontDesc 
	{
		ResourceDesc& resource;
	};

	class Font final: public Resource
	{
	public:
		explicit Font(const FontDesc& desc);
		~Font() override;

		FontAtlas& getFontAtlas();

	private:
		SharedPtr<FontAtlas> m_font;
	};
}


#endif