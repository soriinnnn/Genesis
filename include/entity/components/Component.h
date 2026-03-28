#ifndef GENESIS_COMPONENT_H
#define GENESIS_COMPONENT_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis
{
	struct ComponentDesc
	{
		BaseDesc base;
		Entity& entity;
	};

	class Component: public Base
	{
	public:
		virtual ~Component();

	protected:
		explicit Component(const ComponentDesc& desc);

	protected:
		Entity& m_entity;
	};
}


#endif