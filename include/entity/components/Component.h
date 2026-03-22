#ifndef GENESIS_COMPONENT_H
#define GENESIS_COMPONENT_H
#include <core/Core.h>

namespace genesis
{
	struct ComponentDesc
	{
		Entity& entity;
	};

	class Component
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