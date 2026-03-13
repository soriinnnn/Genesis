#ifndef GENESIS_ENTITY_H
#define GENESIS_ENTITY_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis
{
	struct EntityDesc
	{
		BaseDesc base;
	};

	class Entity: public Base
	{
	public:
		explicit Entity(const EntityDesc& desc);
		virtual ~Entity() override;

	private:

	};
}

#endif