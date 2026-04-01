#include <resources/Texture.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Texture::Texture(const TextureDesc& desc): Resource(desc.resource)
{
	auto& graphicsContext = desc.resource.graphicsContext;

	wstring path{m_path.begin(), m_path.end()};
	m_texture = graphicsContext.graphicsDevice.createImageTexture({path.c_str()});
}

Texture::~Texture() {}

ImageTexture& Texture::getImageTexture() noexcept
{
	return *m_texture;
}