#include <entity/Entity.h>

using namespace genesis;

Entity::Entity(const EntityDesc& desc): Base(desc.base), m_entityManager{desc.entityManager}, m_id {desc.id} {}

Entity::~Entity() {}

EntityId Entity::getId() const noexcept
{
	return m_id;
}

void Entity::update(float deltaTime) 
{
	for (auto& [key, component] : m_components) {
		component->update(deltaTime);
	}
}

void Entity::deleteComponent(ComponentId id)
{
	m_components.erase(id);
}