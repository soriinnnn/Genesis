#ifndef GENESIS_TEXTURE_H
#define GENESIS_TEXTURE_H
#include <resources/Resource.h>

namespace genesis
{
	struct TextureDesc
	{
		GraphicsDevice& graphicsDevice;
	};

	class Texture final: public Resource
	{
	public:
		Texture(const TextureDesc& tDesc, const ResourceDesc& rDesc);
		~Texture() override;

		void load() override;
		void unload() override;

	private:
		SharedPtr<GraphicsTexture> m_graphicsTexture;
	};
}

#endif