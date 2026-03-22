#include <resources/ResourceManager.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Material.h>
#include <core/utils/Macros.h>
#include <filesystem>

using namespace genesis;
using namespace std;

ResourceManager::ResourceManager(const ResourceManagerDesc& desc): Base(desc.base), m_graphicsDevice(desc.graphicsDevice) {}

ResourceManager::~ResourceManager() {}

SharedPtr<Mesh> ResourceManager::getMesh(const char* path)
{
	string absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<Mesh> result = getResource<Mesh>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Mesh, MeshDesc>(id, absolutePath.c_str(), {desc});
	}

	return result;
}

SharedPtr<Material> ResourceManager::getMaterial(const char* path)
{
	string absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<Material> result = getResource<Material>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Material, MaterialDesc>(id, absolutePath.c_str(), {desc});
	}

	return result;
}

SharedPtr<Texture> ResourceManager::getTexture(const char* path)
{
	string absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<Texture> result = getResource<Texture>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Texture, TextureDesc>(id, absolutePath.c_str(), {desc});
	}

	return result;
}

SharedPtr<VertexShader> ResourceManager::getVertexShader(const char* path, const char* entry)
{
	string absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(string{absolutePath + "@" + entry}.c_str());

	SharedPtr<VertexShader> result = getResource<VertexShader>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<VertexShader, VertexShaderDesc>(id, absolutePath.c_str(), {desc, entry});
	}

	return result;
}

SharedPtr<PixelShader> ResourceManager::getPixelShader(const char* path, const char* entry)
{
	string absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(string{absolutePath + "@" + entry}.c_str());

	SharedPtr<PixelShader> result = getResource<PixelShader>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<PixelShader, PixelShaderDesc>(id, absolutePath.c_str(), {desc, entry});
	}

	return result;
}

void ResourceManager::unloadAll()
{
	m_resources.clear();
}

void ResourceManager::unloadUnused()
{
	int unloadedCount = 0;

	auto it = m_resources.begin();
	while (it != m_resources.end()) {
		if (it->second.use_count() == 1) {
			it = m_resources.erase(it);
			unloadedCount++;
		}
		else {
			it++;
		}
	}

	GENESIS_LOG_INFO("Unloaded {} unused resources successfully.", unloadedCount);
}

string ResourceManager::getAbsolutePath(const char* path)
{
	GENESIS_ASSERT(path, "Resource path is null.");
	return filesystem::absolute(path).generic_string();
}

ResourceId ResourceManager::getResourceId(const char* id)
{
	GENESIS_ASSERT(id, "Resource id is null.");
	return hash<string>{}(string{id});
}

ResourceDesc ResourceManager::getResourceDesc(ResourceId id, const char* path)
{
	return ResourceDesc{m_logger, id, path, m_graphicsDevice, *this};
}