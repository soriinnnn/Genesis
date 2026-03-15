#include <resources/ResourceManager.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Texture.h>
#include <resources/Mesh.h>
#include <core/utils/Macros.h>
#include <filesystem>

using namespace genesis;
using namespace std;

ResourceManager::ResourceManager(const ResourceManagerDesc& desc): Base(desc.base), m_graphicsDevice(desc.graphicsDevice) {}

ResourceManager::~ResourceManager() {}

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
	GENESIS_ASSERT(path, "Path is null.");
	string absolutePath;
	string relativePath{path};
	size_t pos = relativePath.find_last_of('@');
	
	if (pos != string::npos) {
		string args = relativePath.substr(pos);
		relativePath = relativePath.substr(0, pos);
		absolutePath = filesystem::absolute(relativePath).generic_string();
		absolutePath += args;
	}
	else {
		absolutePath = filesystem::absolute(relativePath).generic_string();
	}

	return absolutePath;
}

ResourceId ResourceManager::getResourceId(const char* path)
{
	return hash<string>{}(string{path});
}