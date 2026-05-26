#ifndef GENESIS_TEXTURE_H
#define GENESIS_TEXTURE_H
#include <resources/Resource.h>

GENESIS_NAMESPACE_START

struct TextureDesc
{
	ResourceDesc resource;
};

class Texture final: public Resource
{
public:
	explicit Texture(const TextureDesc& desc);
	~Texture() override;

	const ImageTexture& getImageTexture() const noexcept;

private:
	SharedPtr<ImageTexture> m_texture;
};

GENESIS_NAMESPACE_END

#endif