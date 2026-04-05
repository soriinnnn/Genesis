#include <game/World.h>
#include <entity/EntityManager.h>
#include <entity/components/Camera.h>

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

Entity* World::getCamera()
{
	GENESIS_ASSERT(m_camera != nullptr, "Camera is null.");
	return m_camera;
}

void World::setCamera(Entity* camera)
{
	GENESIS_ASSERT(camera != nullptr, "Camera is null.");
	GENESIS_ASSERT(camera->getComponent<Camera>() != nullptr, "Entity must have a Camera component.");
	m_camera = camera;
}