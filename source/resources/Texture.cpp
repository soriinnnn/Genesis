#include <resources/Texture.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Texture::Texture(const TextureDesc& desc): Resource(desc.resource)
{
	auto& graphicsContext = desc.resource.graphicsContext;
	m_texture = graphicsContext.graphicsDevice.createImageTexture({m_path.c_str()});
}

Texture::~Texture() {}

const ImageTexture& Texture::getImageTexture() const noexcept
{
	return *m_texture;
}