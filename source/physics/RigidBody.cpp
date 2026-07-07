#include <physics/RigidBody.h>

using namespace genesis;

RigidBody::RigidBody(const RigidBodyDesc& desc): Base(desc.base), m_entity{nullptr}, m_body{desc.body}, m_bodyInterface{desc.bodyInterface}, m_motionType{desc.motionType}
{
	m_bodyInterface.SetUserData(m_body, reinterpret_cast<uint64>(this));
}

RigidBody::~RigidBody() 
{
	if (!m_body.IsInvalid())
	{
		m_bodyInterface.SetUserData(m_body, 0);
		m_bodyInterface.RemoveBody(m_body);
		m_bodyInterface.DestroyBody(m_body);
	}
}

Entity* RigidBody::getEntity() const
{
	return m_entity;
}

Vec3 RigidBody::getPosition() const
{
	JPH::Vec3 position = m_bodyInterface.GetPosition(m_body);
	return Vec3{position.GetX(), position.GetY(), position.GetZ()};
}

Vec3 RigidBody::getRotation() const
{
	JPH::Vec3 rotation = m_bodyInterface.GetRotation(m_body).GetEulerAngles();
	return Vec3{rotation.GetX(), rotation.GetY(), rotation.GetZ()};
}

Vec3 RigidBody::getLinearVelocity() const
{
	JPH::Vec3 velocity = m_bodyInterface.GetLinearVelocity(m_body);
	return Vec3{velocity.GetX(), velocity.GetY(), velocity.GetZ()};
}

Vec3 RigidBody::getAngularVelocity() const
{
	JPH::Vec3 velocity = m_bodyInterface.GetAngularVelocity(m_body);
	return Vec3{velocity.GetX(), velocity.GetY(), velocity.GetZ()};
}

bool RigidBody::isActive() const
{
	return m_bodyInterface.IsActive(m_body);
}

bool RigidBody::isStatic() const
{
	return m_motionType == MotionType::Static;
}

bool RigidBody::isKinematic() const
{
	return m_motionType == MotionType::Kinematic;
}

bool RigidBody::isDynamic() const
{
	return m_motionType == MotionType::Dynamic;
}

void RigidBody::setPosition(const Vec3& position)
{
	m_bodyInterface.SetPosition(m_body, JPH::Vec3(position.x, position.y, position.z), JPH::EActivation::DontActivate);
}

void RigidBody::setRotation(const Vec3& rotation)
{
	m_bodyInterface.SetRotation(m_body, JPH::Quat::sEulerAngles(JPH::Vec3(rotation.x, rotation.y, rotation.z)), JPH::EActivation::DontActivate);
}

void RigidBody::setLinearVelocity(const Vec3& velocity)
{
	m_bodyInterface.SetLinearVelocity(m_body, JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::setAngularVelocity(const Vec3& velocity)
{
	m_bodyInterface.SetAngularVelocity(m_body, JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::setGravityFactor(float gravity)
{
	m_bodyInterface.SetGravityFactor(m_body, gravity);
}

void RigidBody::addLinearVelocity(const Vec3& velocity)
{
	m_bodyInterface.AddLinearVelocity(m_body, JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::addForce(const Vec3& force)
{
	m_bodyInterface.AddForce(m_body, JPH::Vec3(force.x, force.y, force.z));
}

void RigidBody::addTorque(const Vec3& torque)
{
	m_bodyInterface.AddTorque(m_body, JPH::Vec3(torque.x, torque.y, torque.z), JPH::EActivation::DontActivate);
}

void RigidBody::addImpulse(const Vec3& impulse)
{
	m_bodyInterface.AddImpulse(m_body, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void RigidBody::addAngularImpulse(const Vec3& impulse)
{
	m_bodyInterface.AddAngularImpulse(m_body, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void RigidBody::moveKinematic(const Vec3& position, const Vec3& rotation, float deltaTime)
{
	m_bodyInterface.MoveKinematic(m_body, JPH::Vec3(position.x, position.y, position.z), JPH::Quat::sEulerAngles(JPH::Vec3(rotation.x, rotation.y, rotation.z)), deltaTime);
}

void RigidBody::setOnContactAddedCallback(std::function<void(const ContactAddedData&)> callback)
{
	m_onContactAdded = callback;
}

void RigidBody::setOnContactPersistedCallback(std::function<void(const ContactPersistedData&)> callback)
{
	m_onContactPersisted = callback;
}

void RigidBody::setOnContactRemovedCallback(std::function<void(const ContactRemovedData&)> callback)
{
	m_onContactRemoved = callback;
}

void RigidBody::activate()
{
	m_bodyInterface.ActivateBody(m_body);
}

void RigidBody::deactivate()
{
	m_bodyInterface.DeactivateBody(m_body);
}