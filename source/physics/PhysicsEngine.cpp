#include <physics/PhysicsEngine.h>
#include <physics/RigidBody.h>
#include <physics/Layers.h>
#include <game/World.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <jolt/RegisterTypes.h>
#include <jolt/Core/Factory.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <jolt/Physics/Collision/Shape/BoxShape.h>
#include <iostream>
#include <cstdarg>

#define MAX_BODIES 1024
#define NUM_BODY_MUTEXES 0
#define MAX_BODY_PAIRS 1024
#define MAX_CONTACT_CONSTRAINTS 1024
#define COLLISION_STEPS 1

using namespace genesis;
using namespace std;

static void trace(const char* inFMT, ...)
{
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);
	cout << buffer << endl;
}

PhysicsEngine::PhysicsEngine(const PhysicsEngineDesc& desc): Base(desc.base) 
{
	JPH::RegisterDefaultAllocator();
	JPH::Trace = trace;
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
	
	m_tempAllocator = make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
	m_jobSystem = make_unique<JPH::JobSystemThreadPool>(
		JPH::cMaxPhysicsJobs,
		JPH::cMaxPhysicsBarriers,
		thread::hardware_concurrency() - 1
	);

	m_obpMapping = make_unique<OBPMapping>();
	m_obpCollisionFilter = make_unique<OBPCollisionFilter>();
	m_ooCollisionFilter = make_unique<OOCollisionFilter>();

	m_physicsSystem = make_unique<JPH::PhysicsSystem>();
	m_physicsSystem->Init(
		MAX_BODIES,
		NUM_BODY_MUTEXES,
		MAX_BODY_PAIRS,
		MAX_CONTACT_CONSTRAINTS,
		*m_obpMapping,
		*m_obpCollisionFilter,
		*m_ooCollisionFilter
	);

	m_debugRenderer = make_unique<DebugRenderer>(DebugRendererDesc{desc.graphicsContext});
	m_drawSettings.mDrawShape = true;
	m_drawSettings.mDrawBoundingBox = false;
}

PhysicsEngine::~PhysicsEngine() 
{
	JPH::UnregisterTypes();
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void PhysicsEngine::update(World& world, float deltaTime)
{
	m_physicsSystem->Update(deltaTime, COLLISION_STEPS, m_tempAllocator.get(), m_jobSystem.get());
	world.forEach([&](Entity& entity) {
		auto* rigidBody = entity.getComponent<RigidBodyComponent>();
		auto* transform = entity.getComponent<TransformComponent>();

		if (rigidBody) {
			auto* body = rigidBody->getBody();
			transform->setPosition(body->getPosition());
			transform->setRotation(body->getRotation());
		}
	});
	m_physicsSystem->DrawBodies(m_drawSettings, m_debugRenderer.get());
}

DebugRenderer& PhysicsEngine::getDebugRenderer()
{
	return *m_debugRenderer;
}

SharedPtr<RigidBody> PhysicsEngine::createBox(Vec3 position, Vec3 size, bool isDynamic)
{
	JPH::BoxShapeSettings settings{JPH::Vec3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f)};
	JPH::ShapeSettings::ShapeResult result = settings.Create();
	JPH::ShapeRefC shape = result.Get();

	JPH::ObjectLayer layer = isDynamic ? ObjectLayers::MOVING : ObjectLayers::NON_MOVING;
	JPH::EMotionType motionType = isDynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;

	JPH::BodyCreationSettings bodySettings(
		shape,
		JPH::Vec3(position.x, position.y, position.z),
		JPH::Quat::sIdentity(),
		motionType,
		layer
	);
	
	JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
	JPH::BodyID body = bodyInterface.CreateAndAddBody(bodySettings, JPH::EActivation::Activate);
	
	return make_shared<RigidBody>(RigidBodyDesc{m_logger, body, bodyInterface});
}