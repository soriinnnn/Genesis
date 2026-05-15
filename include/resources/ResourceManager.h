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
		GraphicsContext graphicsContext;
	};

	class ResourceManager final: public Base
	{
	public:
		explicit ResourceManager(const ResourceManagerDesc& desc);
		~ResourceManager() override;

		SharedPtr<Mesh> getMesh(const char* path, uint32 components = GENESIS_VERTEX_PRESET_DEFAULT);
		SharedPtr<Material> getMaterial(const char* path);
		SharedPtr<Texture> getTexture(const char* path);
		SharedPtr<Shader> getShader(const char* path, const char* entry, ShaderType type);
		SharedPtr<PostProcess> getPostProcess(const char* path);
		SharedPtr<Font> getFont(const char* path);

		void unloadResource(ResourceId id);
		void unloadUnused();
		void unloadAll();

	private:
		static ResourceId getResourceId(const char* path);
		static String getAbsolutePath(const char* path);
		ResourceDesc getResourceDesc(ResourceId id, const char* path);

		template<typename T>
		SharedPtr<T> getResource(ResourceId id);

		template<typename T, typename D>
		SharedPtr<T> createResource(ResourceId id, const D& desc);

	private:
		HashMap<ResourceId, SharedPtr<Resource>> m_resources;
		GraphicsContext m_graphicsContext;
	};
}

#include <resources/ResourceManager.inl>
#endif