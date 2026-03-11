#include <resources/Texture.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Texture::Texture(const TextureDesc& tdesc, const ResourceDesc& rdesc): Resource(rdesc), m_graphicsDevice(tdesc.graphicsDevice) {}

Texture::~Texture() {}

GraphicsTexture& Texture::getGraphicsTexture()
{
	if (!m_graphicsTexture) {
		GENESIS_LOG_THROW_ERROR("Texture is not loaded.\nResource: {}", m_path);
	}
	return *m_graphicsTexture;
}

void Texture::load()
{
	if (m_graphicsTexture) {
		GENESIS_LOG_WARNING("Texture is already loaded.\nResource: {}", m_path);
		return;
	}

	wstring path{m_path.begin(), m_path.end()};
	m_graphicsTexture = m_graphicsDevice.createGraphicsTexture({path.c_str()});
}

void Texture::unload()
{
	m_graphicsTexture.reset();
}
