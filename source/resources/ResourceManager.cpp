#include <resources/ResourceManager.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Texture.h>
#include <core/utils/Macros.h>
#include <filesystem>

using namespace genesis;
using namespace std;

static string getAbsolutePath(const char* path);

ResourceManager::ResourceManager(const ResourceManagerDesc& desc): Base(desc.base), m_graphicsDevice(desc.graphicsDevice) {}

ResourceManager::~ResourceManager() {}

SharedPtr<VertexShader> ResourceManager::createVertexShader(const char* path, const char* entryPoint)
{
	string absolutePath = getAbsolutePath(path);
	string key = absolutePath + ":" + entryPoint;

	auto it = m_vertexShaders.find(key);
	if (it != m_vertexShaders.end()) {
		return it->second;
	}

	SharedPtr<VertexShader> shader = make_shared<VertexShader>(VertexShaderDesc{m_graphicsDevice, entryPoint}, getResourceDesc(absolutePath.c_str()));
	shader->load();
	m_vertexShaders.emplace(key, shader);

	return shader;
}

SharedPtr<PixelShader> ResourceManager::createPixelShader(const char* path, const char* entryPoint)
{
	string absolutePath = getAbsolutePath(path);
	string key = absolutePath + ":" + entryPoint;

	auto it = m_pixelShaders.find(key);
	if (it != m_pixelShaders.end()) {
		return it->second;
	}

	SharedPtr<PixelShader> shader = make_shared<PixelShader>(PixelShaderDesc{m_graphicsDevice, entryPoint}, getResourceDesc(absolutePath.c_str()));
	shader->load();
	m_pixelShaders.emplace(key, shader);

	return shader;
}

SharedPtr<Texture> ResourceManager::createTexture(const char* path)
{
	string absolutePath = getAbsolutePath(path);

	auto it = m_textures.find(absolutePath);
	if (it != m_textures.end()) {
		return it->second;
	}

	SharedPtr<Texture> texture = make_shared<Texture>(TextureDesc{m_graphicsDevice}, getResourceDesc(absolutePath.c_str()));
	texture->load();
	m_textures.emplace(absolutePath, texture);

	return texture;
}

void ResourceManager::unloadUnused()
{
	auto cleanup = [](auto& map) {
		for (auto it = map.begin(); it != map.end();) {
			if (it->second.use_count() == 1) {
				it = map.erase(it);
			}
			else {
				++it;
			}
		}
	};

	cleanup(m_vertexShaders);
	cleanup(m_pixelShaders);
	cleanup(m_textures);

	GENESIS_LOG_INFO("Unused resources unloaded successfully.");
}

ResourceDesc ResourceManager::getResourceDesc(const char* path) noexcept
{
	return ResourceDesc{m_logger, path};
}

/* STATIC FUNCTION DEFINITIONS */

static string getAbsolutePath(const char* path) 
{
	GENESIS_ASSERT(path, "Path is null.");
	return filesystem::absolute(path).generic_string();
}