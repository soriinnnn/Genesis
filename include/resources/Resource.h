#ifndef GENESIS_RESOURCE_H
#define GENESIS_RESOURCE_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis 
{
	struct ResourceDesc
	{
		BaseDesc base;
		ResourceId id;
		const char* path;
		GraphicsDevice& graphicsDevice;
		ResourceManager& resourceManager;
	};

	class Resource: public Base
	{
	public:
		~Resource() override;

		ResourceId getId() const noexcept;
		const char* getPath() const noexcept;

	protected:
		explicit Resource(const ResourceDesc& desc);

	protected:
		ResourceId m_id;
		std::string m_path;
	};
}

#endif