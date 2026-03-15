#include <game/World.h>
#include <entity/EntityManager.h>

using namespace genesis;
using namespace std;

World::World(const WorldDesc& desc): Base(desc.base), m_inputManager{desc.inputManager}, m_resourceManager{desc.resourceManager}
{
	m_entityManager = make_unique<EntityManager>(EntityManagerDesc{m_logger});
}

genesis::World::~World() {}