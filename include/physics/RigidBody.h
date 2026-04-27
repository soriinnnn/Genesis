#ifndef GENESIS_RIGID_BODY_H
#define GENESIS_RIGID_BODY_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec3.h>
#include <jolt/Jolt.h>
#include <jolt/Physics/Body/Body.h>
#include <jolt/Physics/Body/BodyInterface.h>

namespace genesis
{
	struct RigidBodyDesc
	{
		BaseDesc base;
		JPH::BodyID body;
		JPH::BodyInterface& bodyInterface;
	};

	class RigidBody final: public Base
	{
	public:
		explicit RigidBody(const RigidBodyDesc& desc);
		~RigidBody() override;

		Vec3 getPosition() const;
		Vec3 getCenterOfMassPosition() const;
		Vec3 getRotation() const;
		Vec3 getLinearVelocity() const;

		void setPosition(const Vec3& position);
		void setLinearVelocity(const Vec3& velocity);
		void addForce(const Vec3& force);
		void addImpulse(const Vec3& impulse);
		void activate();

	private:
		JPH::BodyID m_body;
		JPH::BodyInterface& m_bodyInterface;
	};
}

#endif