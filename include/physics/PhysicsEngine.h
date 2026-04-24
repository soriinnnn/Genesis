#ifndef GENESIS_PHYSICS_ENGINE_H
#define GENESIS_PHYSICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <jolt/Jolt.h>
#include <jolt/Physics/PhysicsSystem.h>

namespace genesis
{
	struct PhysicsEngineDesc
	{
		BaseDesc base;
	};

	class PhysicsEngine final: public Base
	{
	public:
		explicit PhysicsEngine(const PhysicsEngineDesc& desc);
		~PhysicsEngine() override;

	private:
		UniquePtr<JPH::PhysicsSystem> m_physicsSystem;
	};
}

#endif