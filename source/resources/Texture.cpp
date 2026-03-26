#include <resources/Texture.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Texture::Texture(const TextureDesc& desc): Resource(desc.resource)
{
	wstring path{m_path.begin(), m_path.end()};
	m_texture = desc.resource.graphicsDevice.createGraphicsTexture({path.c_str()});
}

Texture::~Texture() {}

GraphicsTexture& Texture::getGraphicsTexture() noexcept
{
	return *m_texture;
}