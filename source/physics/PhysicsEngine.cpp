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
#include <jolt/Physics/Collision/Shape/SphereShape.h>
#include <iostream>
#include <cstdarg>

#define MAX_BODIES 1024
#define NUM_BODY_MUTEXES 0
#define MAX_BODY_PAIRS 1024
#define MAX_CONTACT_CONSTRAINTS 1024
#define TEMP_ALLOCATOR_SIZE (10 * 1024 * 1024)

using namespace genesis;
using namespace std;

static JPH::EMotionType mapMotionTypeToJolt(MotionType motionType);
static JPH::ObjectLayer getObjectLayer(JPH::EMotionType motionType);
static void trace(const char* inFMT, ...);

PhysicsEngine::PhysicsEngine(const PhysicsEngineDesc& desc): Base(desc.base)
{
	JPH::RegisterDefaultAllocator();
	JPH::Trace = trace;
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();

	uint32 hardwareThreads = thread::hardware_concurrency();
	uint32 workerThreads = (hardwareThreads > 0) ? hardwareThreads - 1 : 1;
	m_jobSystem = make_unique<JPH::JobSystemThreadPool>(
		JPH::cMaxPhysicsJobs,
		JPH::cMaxPhysicsBarriers,
		workerThreads
	);

	m_tempAllocator = make_unique<JPH::TempAllocatorImpl>(TEMP_ALLOCATOR_SIZE);
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

#ifdef _DEBUG
	m_debugRenderer = make_unique<DebugRenderer>(DebugRendererDesc{desc.graphicsContext});
	m_drawSettings.mDrawShape = true;
	m_drawSettings.mDrawBoundingBox = false;
#endif
}

PhysicsEngine::~PhysicsEngine() 
{
	JPH::UnregisterTypes();
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void PhysicsEngine::update(EntityManager& entities, float deltaTime)
{
	entities.forEachComponent<RigidBodyComponent>([&](RigidBodyComponent& rigidBody) {
		auto* body = rigidBody.getBody();
		if (!body || !body->isKinematic()) {
			return;
		}

		Entity& entity = rigidBody.getEntity();
		auto* transform = entity.getComponent<TransformComponent>();
		if (!transform) {
			return;
		}
		body->moveKinematic(transform->getPosition(), transform->getRotation(), deltaTime);
	});

	m_physicsSystem->Update(deltaTime, 1, m_tempAllocator.get(), m_jobSystem.get());
	m_contactListener->dispatchEvents();

	entities.forEachComponent<RigidBodyComponent>([](RigidBodyComponent& rigidBody) {
		auto* body = rigidBody.getBody();
		if (!body || !body->isDynamic()) {
			return;
		}

		Entity& entity = rigidBody.getEntity();
		auto* transform = entity.getComponent<TransformComponent>();
		if (!transform) {
			return;
		}
		transform->setPosition(body->getPosition());
		transform->setRotation(body->getRotation());
	});
}

SharedPtr<RigidBody> PhysicsEngine::createBox(Vec3 size, BodySettings settings)
{
	JPH::BoxShapeSettings boxSettings{JPH::Vec3(size.x / 2.0f, size.y / 2.0f, size.z / 2.0f)};
	JPH::ShapeSettings::ShapeResult result = boxSettings.Create();
	if (result.HasError()) {
		GENESIS_LOG_THROW_ERROR("Invalid box shape.\nDetails: {}", result.GetError().c_str());
	}

	JPH::BodyID bodyId = createBody(result.Get(), settings);
	return make_shared<RigidBody>(RigidBodyDesc{m_logger, bodyId, m_physicsSystem->GetBodyInterface(), settings.motionType});
}

SharedPtr<RigidBody> PhysicsEngine::createSphere(float radius, BodySettings settings)
{
	JPH::SphereShapeSettings sphereSettings{radius};
	JPH::ShapeSettings::ShapeResult result = sphereSettings.Create();
	if (result.HasError()) {
		GENESIS_LOG_THROW_ERROR("Invalid sphere shape.\nDetails: {}", result.GetError().c_str());
	}

	JPH::BodyID body = createBody(result.Get(), settings);
	return make_shared<RigidBody>(RigidBodyDesc{m_logger, body, m_physicsSystem->GetBodyInterface(), settings.motionType});
}

SharedPtr<RigidBody> PhysicsEngine::createCapsule(float height, float radius, BodySettings settings)
{
	float halfHeightOfCylinder = (height / 2.0f) - radius;
	JPH::CapsuleShapeSettings capsuleSettings{halfHeightOfCylinder, radius};
	JPH::ShapeSettings::ShapeResult result = capsuleSettings.Create();
	if (result.HasError()) {
		GENESIS_LOG_THROW_ERROR("Invalid capsule shape.\nDetails: {}", result.GetError().c_str());
	}

	JPH::BodyID body = createBody(result.Get(), settings);
	return make_shared<RigidBody>(RigidBodyDesc{m_logger, body, m_physicsSystem->GetBodyInterface(), settings.motionType});
}

#ifdef _DEBUG

void PhysicsEngine::drawDebug()
{
	m_physicsSystem->DrawBodies(m_drawSettings, m_debugRenderer.get());
}

DebugRenderer& PhysicsEngine::getDebugRenderer()
{
	return *m_debugRenderer;
}

#endif

JPH::BodyID PhysicsEngine::createBody(JPH::ShapeRefC shape, BodySettings settings)
{
	JPH::EMotionType type = mapMotionTypeToJolt(settings.motionType);
	JPH::ObjectLayer layer = getObjectLayer(type);

	JPH::BodyCreationSettings bodySettings(
		shape,
		JPH::Vec3(settings.position.x, settings.position.y, settings.position.z),
		JPH::Quat::sIdentity(),
		type,
		layer
	);

	JPH::MassProperties massProperties = bodySettings.GetMassProperties();
	massProperties.mMass = settings.mass;

	bodySettings.mMassPropertiesOverride = massProperties;
	bodySettings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;

	JPH::BodyInterface& bodyInterface = m_physicsSystem->GetBodyInterface();
	return bodyInterface.CreateAndAddBody(bodySettings, JPH::EActivation::Activate);
}

/* STATIC FUNCTIONS DEFINITIONS */

JPH::EMotionType mapMotionTypeToJolt(MotionType motionType)
{
	switch (motionType) {
	case MotionType::Static:
		return JPH::EMotionType::Static;
	case MotionType::Kinematic:
		return JPH::EMotionType::Kinematic;
	case MotionType::Dynamic:
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