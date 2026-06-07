#include <entity/EntityManager.h>
#include <entity/Entity.h>
#include <algorithm>

using namespace genesis;
using namespace std;

EntityManager::EntityManager(const EntityManagerDesc& desc): Base(desc.base) 
{
	m_nextId = 0;
}

EntityManager::~EntityManager() {}

void EntityManager::update()
{
	for (auto& event : m_events) {
		switch (event.type) {
			case EventType::Create: {
				onCreate(event);
				break;
			}
			case EventType::Destroy: {
				onDestroy(event);
				break;
			}	
		}
	}
	m_events.clear();
	m_pendingEntities.clear();
}

bool EntityManager::hasEntity(EntityId id) const
{
	return m_entities.contains(id) || m_pendingEntities.contains(id);
}

Entity* EntityManager::createEntity(const char* name)
{
	Entity* result = getEntityByName(name);

	if (!result) {
		EntityId id = getAvailableEntityId();
		UniquePtr<Entity> entity = make_unique<Entity>(EntityDesc{m_logger, id, name, *this});
		result = entity.get();

		m_pendingEntities.emplace(id, move(entity));
		m_events.push_back({EventType::Create, id});
		if (name) {
			m_nameToId.emplace(name, id);
		}
	}
	else {
		GENESIS_LOG_WARNING("Entity with name \"{}\" already exists. Returning existing entity.", name);
	}

	return result;
}

void EntityManager::destroyEntity(EntityId id)
{
	auto it = m_entities.find(id);
	if (it == m_entities.end()) {
		GENESIS_LOG_WARNING("Trying to destroy nonexistent entity.");
		return;
	}
	m_events.push_back({EventType::Destroy, id});
}

Entity* EntityManager::getEntity(EntityId id)
{
	auto entity = m_entities.find(id);
	if (entity != m_entities.end()) {
		return entity->second.get();
	}

	auto pending = m_pendingEntities.find(id);
	if (pending != m_pendingEntities.end()) {
		return pending->second.get();
	}

	return nullptr;
}

bool EntityManager::hasEntityByName(const char* name) const
{
	if (!name) {
		return false;
	}
	return m_nameToId.contains(name);
}

void EntityManager::destroyEntityByName(const char* name)
{
	if (!name) {
		return;
	}

	auto it = m_nameToId.find(name);
	if (it == m_nameToId.end()) {
		GENESIS_LOG_WARNING("Trying to destroy nonexistent entity.");
		return;
	}
	m_events.push_back({EventType::Destroy, it->second});
}

Entity* EntityManager::getEntityByName(const char* name)
{
	if (!name) {
		return nullptr;
	}

	auto it1 = m_nameToId.find(name);
	if (it1 == m_nameToId.end()) {
		return nullptr;
	}

	auto it2 = m_entities.find(it1->second);
	if (it2 != m_entities.end()) {
		return it2->second.get();
	}

	it2 = m_pendingEntities.find(it1->second);
	if (it2 != m_pendingEntities.end()) {
		return it2->second.get();
	}

	return nullptr;
}

EntityId EntityManager::getAvailableEntityId()
{
	EntityId id;	

	if (m_idPool.empty()) {
		id = m_nextId;
		m_nextId++;
	}
	else {
		id = m_idPool.back();
		m_idPool.pop_back();
	}

	return id;
}

void EntityManager::registerComponent(TypeId id, Component* component)
{
	m_components[id].push_back(component);
}

void EntityManager::unregisterComponent(TypeId id, Component* component)
{
	auto it = m_components.find(id);
	if (it == m_components.end()) {
		return;
	}
	auto& components = it->second;

	int i = 0;
	bool found = false;
	while (i < components.size() && !found) {
		if (components[i] == component) {
			found = true;
		}
		else {
			i++;
		}
	}
	if (found) {
		components[i] = components.back();
		components.pop_back();
	}
}

void EntityManager::onCreate(EntityEvent event)
{
	m_entities.emplace(event.id, move(m_pendingEntities[event.id]));
	m_pendingEntities.erase(event.id);
}

void EntityManager::onDestroy(EntityEvent event)
{
	auto it = m_entities.find(event.id);
	if (it != m_entities.end()) {
		m_nameToId.erase(it->second->getName());
	}

	m_entities.erase(event.id);
	m_idPool.push_back(event.id);
}