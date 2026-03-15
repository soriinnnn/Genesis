#ifndef GENESIS_ENTITY_H
#define GENESIS_ENTITY_H
#include <core/Base.h>
#include <core/Core.h>

namespace genesis
{
	struct EntityDesc
	{
		BaseDesc base;
		EntityManager& entityManager;
		EntityId id;
	};

	class Entity: public Base
	{
	public:
		virtual ~Entity() override;

		EntityId getId() const noexcept;
		virtual void update(float deltaTime) = 0;

	protected:
		explicit Entity(const EntityDesc& desc);

	private:
		EntityManager& m_entityManager;
		EntityId m_id;
	};
}

#endif