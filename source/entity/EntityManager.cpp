#include <entity/EntityManager.h>

using namespace genesis;
using namespace std;

EntityManager::EntityManager(const EntityManagerDesc& desc): Base(desc.base), m_nextId{0} {}

EntityManager::~EntityManager() {}

const HashMap<EntityId, UniquePtr<Entity>>& EntityManager::getEntities() const
{
	return m_entities;
}

void EntityManager::destroyEntity(EntityId id)
{
	if (!m_entities.contains(id)) {
		GENESIS_LOG_WARNING("Trying to destroy nonexistent entity.");
		return;
	}
	m_entitiesToDestroy.push_back(id);
}

void EntityManager::destroyPending()
{
	while (!m_entitiesToDestroy.empty()) {
		EntityId entity = m_entitiesToDestroy.back();
		m_entitiesToDestroy.pop_back();

		m_entities.erase(entity);
		m_idPool.push_back(entity);
	}
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