#ifndef GENESIS_TEXTURE_H
#define GENESIS_TEXTURE_H
#include <resources/Resource.h>

namespace genesis
{
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
}

#endif