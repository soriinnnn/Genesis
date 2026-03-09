#include <resources/Texture.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Texture::Texture(const TextureDesc& tDesc, const ResourceDesc& rDesc): Resource(rDesc)
{
	wstring path{m_path.begin(), m_path.end()};
	m_graphicsTexture = tDesc.graphicsDevice.createGraphicsTexture({path.c_str()});
}

Texture::~Texture() {}

void Texture::load()
{
}

void Texture::unload()
{
}
