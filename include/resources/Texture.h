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
		Texture(const TextureDesc& tdesc, const ResourceDesc& rdesc);
		~Texture() override;

		GraphicsTexture& getGraphicsTexture();

		void load() override;
		void unload() override;

	private:
		GraphicsDevice& m_graphicsDevice;
		SharedPtr<GraphicsTexture> m_graphicsTexture;
	};
}

#endif