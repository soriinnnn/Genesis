#include <physics/PhysicsEngine.h>

using namespace genesis;
using namespace std;

PhysicsEngine::PhysicsEngine(const PhysicsEngineDesc& desc): Base(desc.base) 
{
	m_physicsSystem = make_unique<JPH::PhysicsSystem>();
}

PhysicsEngine::~PhysicsEngine() {}