#include <resources/ResourceManager.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>
#include <resources/Shader.h>
#include <resources/Material.h>
#include <resources/PostProcess.h>
#include <resources/Font.h>
#include <resources/SkyBox.h>
#include <core/utils/Macros.h>
#include <filesystem>

using namespace genesis;
using namespace std;

ResourceManager::ResourceManager(const ResourceManagerDesc& desc): Base(desc.base), m_graphicsContext{desc.graphicsContext} {}

ResourceManager::~ResourceManager() {}

SharedPtr<Mesh> ResourceManager::getMesh(const char* path, uint32 components)
{
	String absolutePath = getAbsolutePath(path);
	String uniqueKey = absolutePath + "@" + to_string(components);
	ResourceId id = getResourceId(uniqueKey.c_str());

	SharedPtr<Mesh> result = getResource<Mesh>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Mesh, MeshDesc>(id, {desc, components});
	}

	return result;
}

SharedPtr<Material> ResourceManager::getMaterial(const char* path)
{
	String absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<Material> result = getResource<Material>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Material, MaterialDesc>(id, {desc});
	}

	return result;
}

SharedPtr<Texture> ResourceManager::getTexture(const char* path)
{
	String absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<Texture> result = getResource<Texture>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Texture, TextureDesc>(id, {desc});
	}

	return result;
}

SharedPtr<Shader> ResourceManager::getShader(const char* path, const char* entry, ShaderType type)
{
	String absolutePath = getAbsolutePath(path);
	String uniqueKey = absolutePath + "@" + entry + "@" + to_string(static_cast<int>(type));
	ResourceId id = getResourceId(uniqueKey.c_str());

	SharedPtr<Shader> result = getResource<Shader>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Shader, ShaderDesc>(id, {desc, entry, type});
	}

	return result;
}

SharedPtr<PostProcess> ResourceManager::getPostProcess(const char* path)
{
	String absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<PostProcess> result = getResource<PostProcess>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<PostProcess, PostProcessDesc>(id, {desc});
	}

	return result;
}

SharedPtr<Font> ResourceManager::getFont(const char* path)
{
	String absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<Font> result = getResource<Font>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<Font, FontDesc>(id, {desc});
	}

	return result;
}

SharedPtr<SkyBox> ResourceManager::getSkybox(const char* path)
{
	String absolutePath = getAbsolutePath(path);
	ResourceId id = getResourceId(absolutePath.c_str());

	SharedPtr<SkyBox> result = getResource<SkyBox>(id);
	if (!result) {
		ResourceDesc desc = getResourceDesc(id, absolutePath.c_str());
		result = createResource<SkyBox, SkyBoxDesc>(id, {desc});
	}

	return result;
}

void ResourceManager::unloadResource(ResourceId id)
{
	m_resources.erase(id);
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

	if (unloadedCount > 0) {
		GENESIS_LOG_INFO("Unloaded {} unused resources successfully.", unloadedCount);
	}
}

void ResourceManager::unloadAll()
{
	m_resources.clear();
}

String ResourceManager::getAbsolutePath(const char* path)
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
	return ResourceDesc{m_logger, id, path, *this, m_graphicsContext};
}