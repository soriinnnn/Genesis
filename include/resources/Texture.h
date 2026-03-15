#ifndef GENESIS_TEXTURE_H
#define GENESIS_TEXTURE_H
#include <resources/Resource.h>

namespace genesis
{
	class Texture final: public Resource
	{
	public:
		Texture(const ResourceDesc& desc);
		~Texture() override;

		GraphicsTexture& getGraphicsTexture();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		SharedPtr<GraphicsTexture> m_graphicsTexture;
	};
}

#endif