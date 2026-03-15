#include <resources/Texture.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Texture::Texture(const ResourceDesc& desc): Resource(desc) {}

Texture::~Texture() {}

GraphicsTexture& Texture::getGraphicsTexture()
{
	if (!isLoaded()) {
		GENESIS_LOG_THROW_ERROR("Resource is not loaded: {}", m_path);
	}
	return *m_graphicsTexture;
}

void Texture::onLoad()
{
	wstring path{m_path.begin(), m_path.end()};
	m_graphicsTexture = m_graphicsDevice.createGraphicsTexture({path.c_str()});
}

void Texture::onUnload()
{
	m_graphicsTexture.reset();
}