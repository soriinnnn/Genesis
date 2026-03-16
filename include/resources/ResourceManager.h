#ifndef GENESIS_RESOURCE_MANAGER_H
#define GENESIS_RESOURCE_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <resources/Resource.h>

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

		template<typename T>
		SharedPtr<T> getResource(const char* path)
		{
			GENESIS_ASSERT((std::is_base_of<Resource, T>::value), "T must derive from Resource.");
			std::string absolutePath = getAbsolutePath(path);
			ResourceId id = getResourceId(absolutePath.c_str());
			
			SharedPtr<T> result = getResource<T>(id);
			if (!result) {
				result = createResource<T>(id, absolutePath.c_str());
			}
			if (!result->isLoaded()) {
				result->load();
			}

			return result;
		}

		void unloadAll();
		void unloadUnused();

	private:
		static std::string getAbsolutePath(const char* path);
		static ResourceId getResourceId(const char* path);

		template<typename T>
		SharedPtr<T> getResource(ResourceId id)
		{
			auto it = m_resources.find(id);
			if (it == m_resources.end()) {
				return nullptr;
			}
			return static_pointer_cast<T>(it->second);
		}

		template<typename T>
		SharedPtr<T> createResource(ResourceId id, const char* path)
		{
			SharedPtr<T> resource = std::make_shared<T>(ResourceDesc{m_logger, m_graphicsDevice, path});
			m_resources.emplace(id, resource);

			return resource;
		}

	private:
		std::unordered_map<ResourceId, SharedPtr<Resource>> m_resources;
		GraphicsDevice& m_graphicsDevice;
	};
}

#endif