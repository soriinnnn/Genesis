#ifndef GENESIS_RIGID_BODY_H
#define GENESIS_RIGID_BODY_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec3.h>
#include <jolt/Jolt.h>
#include <jolt/Physics/Body/Body.h>
#include <jolt/Physics/Body/BodyInterface.h>

GENESIS_NAMESPACE_BEGIN

struct RigidBodyDesc
{
	BaseDesc base;
	JPH::BodyID body;
	JPH::BodyInterface& bodyInterface;
};

class RigidBody final: public Base
{
public:
	struct ContactAddedData {
		RigidBody* other;
		Vec3 contactPoint;
		Vec3 contactNormal;
		float penetrationDepth;
	};

	struct ContactPersistedData {
		RigidBody* other;
		Vec3 contactPoint;
		Vec3 contactNormal;
		float penetrationDepth;
	};

	struct ContactRemovedData {
		RigidBody* other;
	};

public:
	explicit RigidBody(const RigidBodyDesc& desc);
	~RigidBody() override;

	Entity* getEntity() const;
	Vec3 getPosition() const;
	Vec3 getRotation() const;
	Vec3 getLinearVelocity() const;
	Vec3 getAngularVelocity() const;
	bool isActive() const;

	void setPosition(const Vec3& position);
	void setRotation(const Vec3& rotation);
	void setLinearVelocity(const Vec3& velocity);
	void setAngularVelocity(const Vec3& velocity);

	void addLinearVelocity(const Vec3& velocity);
	void addForce(const Vec3& force);
	void addTorque(const Vec3& torque);
	void addImpulse(const Vec3& impulse);
	void addAngularImpulse(const Vec3& impulse);
	void moveKinematic(const Vec3& position, const Vec3& rotation, float deltaTime);

	void setOnContactAddedCallback(std::function<void(const ContactAddedData&)> callback);
	void setOnContactPersistedCallback(std::function<void(const ContactPersistedData&)> callback);
	void setOnContactRemovedCallback(std::function<void(const ContactRemovedData&)> callback);

	void activate();
	void deactivate();

private:
	Entity* m_entity;
	JPH::BodyID m_body;
	JPH::BodyInterface& m_bodyInterface;

private:
	std::function<void(ContactAddedData&)> m_onContactAdded;
	std::function<void(const ContactPersistedData&)> m_onContactPersisted;
	std::function<void(const ContactRemovedData&)> m_onContactRemoved;

	friend class ContactListener;
	friend class RigidBodyComponent;
};

GENESIS_NAMESPACE_END

#endif