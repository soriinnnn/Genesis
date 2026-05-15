#include <physics/PhysicsEngine.h>
#include <physics/RigidBody.h>
#include <physics/Layers.h>
#include <entity/EntityManager.h>
#include <entity/Entity.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <jolt/RegisterTypes.h>
#include <jolt/Core/Factory.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <jolt/Physics/Collision/Shape/BoxShape.h>
#include <jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <iostream>
#include <cstdarg>

#define MAX_BODIES 1024
#define NUM_BODY_MUTEXES 0
#define MAX_BODY_PAIRS 1024
#define MAX_CONTACT_CONSTRAINTS 1024
#define MAX_COLLISION_STEPS 5

constexpr float UPDATE_RATE = 1.0f / 60.0f;

using namespace genesis;
using namespace std;

static JPH::EMotionType mapMotionTypeToJolt(PhysicsEngine::MotionType motionType);
static JPH::ObjectLayer getObjectLayer(JPH::EMotionType motionType);
static void trace(const char* inFMT, ...);

PhysicsEngine::PhysicsEngine(const PhysicsEngineDesc& desc): Base(desc.base), m_accumulator{0.0f}
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

	m_contactListener = make_unique<ContactListener>(ContactListenerDesc{m_physicsSystem->GetBodyInterface()});
	m_physicsSystem->SetContactListener(m_contactListener.get());

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

void PhysicsEngine::update(EntityManager& entities, float deltaTime)
{
	entities.forEachComponent<RigidBodyComponent>([](RigidBodyComponent& rigidBody) {
		Entity& entity = rigidBody.getEntity();
		auto* transform = entity.getComponent<TransformComponent>();

		auto* body = rigidBody.getBody();
		body->setPosition(transform->getPosition());
		body->setRotation(transform->getRotation());
	});

	m_accumulator += deltaTime;

	int steps = 0;
	while (m_accumulator >= UPDATE_RATE && steps < MAX_COLLISION_STEPS) {
		steps += 1;
		m_accumulator -= UPDATE_RATE;
	}
	if (steps == MAX_COLLISION_STEPS) {
		m_accumulator = 0.0f;
	}

	if (steps > 0) {
		m_physicsSystem->Update(UPDATE_RATE * steps, steps, m_tempAllocator.get(), m_jobSystem.get());
		m_contactListener->dispatchEvents();

		entities.forEachComponent<RigidBodyComponent>([](RigidBodyComponent& rigidBody) {
			Entity& entity = rigidBody.getEntity();
			auto* transform = entity.getComponent<TransformComponent>();

			auto* body = rigidBody.getBody();
			transform->setPosition(body->getPosition());
			transform->setRotation(body->getRotation());
		});
	}

#ifdef _DEBUG
	m_physicsSystem->DrawBodies(m_drawSettings, m_debugRenderer.get());
#endif
}

DebugRenderer& PhysicsEngine::getDebugRenderer()
{
	return *m_debugRenderer;
}

SharedPtr<RigidBody> PhysicsEngine::createBox(Vec3 position, Vec3 size, MotionType motionType)
{
	JPH::BoxShapeSettings settings{JPH::Vec3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f)};
	JPH::ShapeSettings::ShapeResult result = settings.Create();
	if (result.HasError()) {
		GENESIS_LOG_THROW_ERROR("Invalid box shape.\nDetails: {}", result.GetError().c_str());
	}

	JPH::ShapeRefC shape = result.Get();
	JPH::EMotionType type = mapMotionTypeToJolt(motionType);
	JPH::ObjectLayer layer = getObjectLayer(type);

	JPH::BodyCreationSettings bodySettings(
		shape,
		JPH::Vec3(position.x, position.y, position.z),
		JPH::Quat::sIdentity(),
		type,
		layer
	);

	JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
	JPH::BodyID body = bodyInterface.CreateAndAddBody(bodySettings, JPH::EActivation::Activate);
	
	return make_shared<RigidBody>(RigidBodyDesc{m_logger, body, bodyInterface});
}

SharedPtr<RigidBody> PhysicsEngine::createCapsule(Vec3 position, float height, float radius, MotionType motionType)
{
	float halfHeightOfCylinder = (height / 2.0f) - radius;
	JPH::CapsuleShapeSettings settings{halfHeightOfCylinder, radius};
	JPH::ShapeSettings::ShapeResult result = settings.Create();
	if (result.HasError()) {
		GENESIS_LOG_THROW_ERROR("Invalid capsule shape.\nDetails: {}", result.GetError().c_str());
	}

	JPH::ShapeRefC shape = result.Get();
	JPH::EMotionType type = mapMotionTypeToJolt(motionType);
	JPH::ObjectLayer layer = getObjectLayer(type);

	JPH::BodyCreationSettings bodySettings(
		shape,
		JPH::Vec3(position.x, position.y, position.z),
		JPH::Quat::sIdentity(),
		type,
		layer
	);

	JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
	JPH::BodyID body = bodyInterface.CreateAndAddBody(bodySettings, JPH::EActivation::Activate);

	return make_shared<RigidBody>(RigidBodyDesc{m_logger, body, bodyInterface});
}

/* STATIC FUNCTIONS DEFINITIONS */

JPH::EMotionType mapMotionTypeToJolt(PhysicsEngine::MotionType motionType)
{
	switch (motionType) {
	case PhysicsEngine::MotionType::Static:
		return JPH::EMotionType::Static;
	case PhysicsEngine::MotionType::Kinematic:
		return JPH::EMotionType::Kinematic;
	case PhysicsEngine::MotionType::Dynamic:
		return JPH::EMotionType::Dynamic;
	default:
		return JPH::EMotionType::Static;
	}
}

JPH::ObjectLayer getObjectLayer(JPH::EMotionType motionType)
{
	switch (motionType) {
	case JPH::EMotionType::Static:
		return ObjectLayers::NON_MOVING;
	case JPH::EMotionType::Kinematic:
		return ObjectLayers::MOVING;
	case JPH::EMotionType::Dynamic:
		return ObjectLayers::MOVING;
	default:
		return ObjectLayers::NON_MOVING;
	}
}

void trace(const char* inFMT, ...)
{
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);
	cout << buffer << endl;
}