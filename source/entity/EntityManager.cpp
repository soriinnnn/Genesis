#include <entity/EntityManager.h>
#include <algorithm>

using namespace genesis;
using namespace std;

EntityManager::EntityManager(const EntityManagerDesc& desc): Base(desc.base), m_nextId{0} {}

EntityManager::~EntityManager() {}

void EntityManager::update(float deltaTime)
{
	for (auto& event : m_events) {
		switch (event.type) {
		case EventType::Create: 
			m_entities.emplace(event.id, std::move(m_pendingEntities[event.id]));
			break;
		case EventType::Destroy:
			m_entities.erase(event.id);
			m_idPool.push_back(event.id);
			break;
		}
	}
	m_events.clear();
	m_pendingEntities.clear();

	for (auto& [key, entity] : m_entities) {
		entity->update(deltaTime);
	}
}

Entity* EntityManager::createEntity()
{
	EntityId id = getAvailableId();
	UniquePtr<Entity> entity = make_unique<Entity>(EntityDesc{m_logger, id, *this});
	Entity* result = entity.get();

	m_pendingEntities.emplace(id, move(entity));
	m_events.push_back({EventType::Create, id});
	
	return result;
}

Entity* EntityManager::getEntity(EntityId id)
{
	auto it = m_entities.find(id);
	if (it == m_entities.end()) {
		return nullptr;
	}
	return it->second.get();
}

void EntityManager::destroyEntity(EntityId id)
{
	if (!m_entities.contains(id)) {
		GENESIS_LOG_WARNING("Trying to destroy nonexistent entity.");
		return;
	}
	m_events.push_back({EventType::Destroy, id});
}

EntityId EntityManager::getAvailableId()
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