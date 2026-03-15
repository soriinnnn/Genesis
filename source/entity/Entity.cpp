#include <entity/Entity.h>

using namespace genesis;

Entity::Entity(const EntityDesc& desc): Base(desc.base), m_entityManager{desc.entityManager}, m_id {desc.id} {}

Entity::~Entity() {}

EntityId Entity::getId() const noexcept
{
	return m_id;
}
