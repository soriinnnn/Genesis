#include <game/World.h>
#include <entity/EntityManager.h>
#include <entity/components/CameraComponent.h>

using namespace genesis;
using namespace std;

World::World(const WorldDesc& desc): Base(desc.base), m_camera{nullptr}
{
	m_entityManager = make_unique<EntityManager>(EntityManagerDesc{m_logger});
}

World::~World() {}

void World::update(float deltaTime)
{
	m_entityManager->update(deltaTime);
}

Entity* World::createEntity()
{
	return m_entityManager->createEntity();
}

Entity* World::getEntity(EntityId id)
{
	return m_entityManager->getEntity(id);
}

Entity* World::createEntity(const char* name)
{
	if (m_entityMapping.contains(name)) {
		GENESIS_LOG_THROW_ERROR("Entity with name \"{}\" already exists.", name);
	}
	Entity* entity = m_entityManager->createEntity();
	m_entityMapping.emplace(name, entity->getId());
	return entity;
}

Entity* World::getEntity(const char* name)
{
	auto it = m_entityMapping.find(name);
	if (it == m_entityMapping.end()) {
		return nullptr;
	}
	return m_entityManager->getEntity(it->second);
}

Entity* World::getCamera()
{
	GENESIS_ASSERT(m_camera != nullptr, "Camera is null.");
	return m_camera;
}

void World::setCamera(Entity* camera)
{
	GENESIS_ASSERT(camera != nullptr, "Camera is null.");
	GENESIS_ASSERT(camera->getComponent<CameraComponent>() != nullptr, "Entity must have a Camera component.");
	m_camera = camera;
}