#include <resources/ResourceManager.h>
#include <resources/Texture.h>
#include <core/utils/Macros.h>
#include <filesystem>

using namespace genesis;
using namespace std;

static string getAbsolutePath(const char* path);

ResourceManager::ResourceManager(const ResourceManagerDesc& desc): Base(desc.base), m_graphicsDevice(desc.graphicsDevice) {}

ResourceManager::~ResourceManager() {}

SharedPtr<Texture> ResourceManager::createTexture(const char* path)
{
	string absolutePath = getAbsolutePath(path);

	auto it = m_textures.find(absolutePath);
	if (it != m_textures.end()) {
		return it->second;
	}

	SharedPtr<Texture> texture = make_shared<Texture>(TextureDesc{m_graphicsDevice}, getResourceDesc(path));
	m_textures.emplace(absolutePath, texture);

	return texture;
}

ResourceDesc ResourceManager::getResourceDesc(const char* path) noexcept
{
	return ResourceDesc{m_logger, path};
}

/* STATIC FUNCTION DEFINITIONS */

static string getAbsolutePath(const char* path) 
{
	GENESIS_ASSERT(path, "Path is null.");
	return filesystem::absolute(path).string();
}