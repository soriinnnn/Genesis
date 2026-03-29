#ifndef GENESIS_RESOURCE_MANAGER_H
#define GENESIS_RESOURCE_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <resources/Resource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct ResourceManagerDesc
	{
		BaseDesc base;
		GraphicsDevice& graphicsDevice;
	};

	class ResourceManager final: public Base
	{
	public:
		explicit ResourceManager(const ResourceManagerDesc& desc);
		~ResourceManager() override;

		SharedPtr<Mesh> getMesh(const char* path, uint32 components = GENESIS_VERTEX_DEFAULT);
		SharedPtr<Material> getMaterial(const char* path);
		SharedPtr<Texture> getTexture(const char* path);
		SharedPtr<Shader> getShader(const char* path, const char* entry, ShaderType type);

		void unloadResource(ResourceId id);
		void unloadUnused();
		void unloadAll();

	private:
		static ResourceId getResourceId(const char* path);
		static std::string getAbsolutePath(const char* path);
		ResourceDesc getResourceDesc(ResourceId id, const char* path);

		template<typename T>
		SharedPtr<T> getResource(ResourceId id)
		{
			auto it = m_resources.find(id);
			if (it == m_resources.end()) {
				return nullptr;
			}
			return dynamic_pointer_cast<T>(it->second);
		}

		template<typename T, typename D>
		SharedPtr<T> createResource(ResourceId id, const char* path, const D& desc)
		{
			SharedPtr<T> resource = std::make_shared<T>(desc);
			m_resources.emplace(id, resource);
			return resource;
		}

	private:
		std::unordered_map<ResourceId, SharedPtr<Resource>> m_resources;
		GraphicsDevice& m_graphicsDevice;
	};
}

#endif