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

		SharedPtr<Texture> createTexture(const char* path);

	private:
		ResourceDesc getResourceDesc(const char* path) noexcept;

	private:
		GraphicsDevice& m_graphicsDevice;
		std::unordered_map<std::string, SharedPtr<Texture>> m_textures;
	};
}

#endif