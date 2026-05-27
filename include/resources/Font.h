#ifndef GENESIS_FONT_H
#define GENESIS_FONT_H
#include <resources/Resource.h>

GENESIS_NAMESPACE_BEGIN

struct FontDesc
{
	ResourceDesc& resource;
};

class Font final: public Resource
{
public:
	explicit Font(const FontDesc& desc);
	~Font() override;

	FontAtlas& getAtlas();

private:
	SharedPtr<FontAtlas> m_atlas;
};

GENESIS_NAMESPACE_END

#endif