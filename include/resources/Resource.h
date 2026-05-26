#ifndef GENESIS_RESOURCE_H
#define GENESIS_RESOURCE_H
#include <core/Base.h>
#include <core/Core.h>
#include <graphics/utils/GraphicsTypes.h>

GENESIS_NAMESPACE_START

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

GENESIS_NAMESPACE_END

#endif