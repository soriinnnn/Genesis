#ifndef GENESIS_RESOURCE_H
#define GENESIS_RESOURCE_H
#include <core/Base.h>
#include <core/Core.h>
#include <core/utils/Types.h>

namespace genesis 
{
	struct ResourceDesc
	{
		BaseDesc base;
		ResourceId id;
		const char* path;
		ResourceManager& resourceManager;
		GraphicsContext graphicsContext;
	};

	class Resource: public Base
	{
	public:
		virtual ~Resource() override;

		ResourceId getId() const noexcept;
		const char* getPath() const noexcept;

	protected:
		explicit Resource(const ResourceDesc& desc);

	protected:
		ResourceId m_id;
		String m_path;
	};
}

#endif