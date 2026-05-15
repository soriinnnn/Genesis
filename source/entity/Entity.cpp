#include <entity/Entity.h>

using namespace genesis;
using namespace std;

Entity::Entity(const EntityDesc& desc): Base(desc.base), m_id{desc.id}, m_manager{desc.manager} 
{
	m_name = (desc.name) ? String(desc.name) : String();
}

Entity::~Entity() 
{
	for (auto& [id, component] : m_components) {
		m_manager.unregisterComponent(id, component.get());
	}
}

void Entity::update(float deltaTime)
{
	for (auto& [id, component] : m_components) {
		component->update(deltaTime);
	}
}

EntityId Entity::getId() const noexcept
{
	return m_id;
}

const char* Entity::getName() const noexcept
{
	return m_name.c_str();
}