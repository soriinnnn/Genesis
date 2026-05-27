#ifndef GENESIS_COMPONENT_H
#define GENESIS_COMPONENT_H
#include <core/Base.h>
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

struct ComponentDesc
{
	BaseDesc base;
	Entity& entity;
};

class Component: public Base
{
public:
	virtual ~Component();

	Entity& getEntity();

protected:
	explicit Component(const ComponentDesc& desc);

protected:
	Entity& m_entity;
};

GENESIS_NAMESPACE_END

#endif