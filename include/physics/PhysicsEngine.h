#ifndef GENESIS_PHYSICS_ENGINE_H
#define GENESIS_PHYSICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec3.h>
#include <jolt/Jolt.h>
#include <jolt/Core/TempAllocator.h>
#include <jolt/Core/JobSystemThreadPool.h>
#include <jolt/Physics/PhysicsSystem.h>
#include <physics/OBPMapping.h>
#include <physics/OBPCollisionFilter.h>
#include <physics/OOCollisionFilter.h>
#include <physics/ContactListener.h>
#include <physics/utils/PhysicsTypes.h>
#include <graphics/utils/GraphicsTypes.h>

#ifdef _DEBUG
#include <physics/DebugRenderer.h>
#endif

GENESIS_NAMESPACE_BEGIN

struct PhysicsEngineDesc
{
	BaseDesc base;
	GraphicsContext graphicsContext;
};

class PhysicsEngine final: public Base
{
public:
	explicit PhysicsEngine(const PhysicsEngineDesc& desc);
	~PhysicsEngine() override;

	void update(EntityManager& entities, float deltaTime);

	SharedPtr<RigidBody> createBox(Vec3 position, Vec3 size, MotionType motionType, float mass = 1.0f);
	SharedPtr<RigidBody> createCapsule(Vec3 position, float height, float radius, MotionType motionType, float mass = 1.0f);

#ifdef _DEBUG
	void drawDebug();
	DebugRenderer& getDebugRenderer();
#endif

private:
	UniquePtr<JPH::TempAllocatorImpl> m_tempAllocator;
	UniquePtr<JPH::JobSystemThreadPool> m_jobSystem;
	UniquePtr<JPH::PhysicsSystem> m_physicsSystem;
	UniquePtr<OBPMapping> m_obpMapping;
	UniquePtr<OBPCollisionFilter> m_obpCollisionFilter;
	UniquePtr<OOCollisionFilter> m_ooCollisionFilter;
	UniquePtr<ContactListener> m_contactListener;

#ifdef _DEBUG
	UniquePtr<DebugRenderer> m_debugRenderer;
	JPH::BodyManager::DrawSettings m_drawSettings;
#endif

};

GENESIS_NAMESPACE_END

#endif