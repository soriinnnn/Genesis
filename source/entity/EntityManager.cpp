#include <entity/EntityManager.h>

using namespace genesis;

EntityManager::EntityManager(const EntityManagerDesc& desc): Base(desc.base)
{
    m_nextEntityId = 0;
}

EntityManager::~EntityManager() {}

uint32 EntityManager::createEntity()
{
    uint32 entityId;

    if (m_availableEntityId.empty()) {
        entityId = m_availableEntityId.front();
        m_availableEntityId.pop();
    }
    else {
        entityId = m_nextEntityId;
        m_nextEntityId++;
    }

    return entityId;
}

void EntityManager::destroyEntity(uint32 entityId)
{



    m_availableEntityId.push(entityId);
}
