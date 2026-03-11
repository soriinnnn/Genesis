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

		SharedPtr<VertexShader> createVertexShader(const char* path, const char* entryPoint);
		SharedPtr<PixelShader> createPixelShader(const char* path, const char* entryPoint);
		SharedPtr<Texture> createTexture(const char* path);
		void unloadUnused();

	private:
		ResourceDesc getResourceDesc(const char* path) noexcept;

	private:
		GraphicsDevice& m_graphicsDevice;
		std::unordered_map<std::string, SharedPtr<VertexShader>> m_vertexShaders;
		std::unordered_map<std::string, SharedPtr<PixelShader>> m_pixelShaders;
		std::unordered_map<std::string, SharedPtr<Texture>> m_textures;
	};
}

#endif