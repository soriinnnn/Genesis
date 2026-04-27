#include <physics/RigidBody.h>

using namespace genesis;

RigidBody::RigidBody(const RigidBodyDesc& desc): Base(desc.base), m_body{desc.body}, m_bodyInterface{desc.bodyInterface} {}

RigidBody::~RigidBody() 
{
	m_bodyInterface.RemoveBody(m_body);
	m_bodyInterface.DestroyBody(m_body);
}

Vec3 RigidBody::getPosition() const
{
	JPH::Vec3 position = m_bodyInterface.GetPosition(m_body);
	return Vec3{position.GetX(), position.GetY(), position.GetZ()};
}

Vec3 RigidBody::getCenterOfMassPosition() const
{
	JPH::Vec3 position = m_bodyInterface.GetCenterOfMassPosition(m_body);
	return Vec3{position.GetX(), position.GetY(), position.GetZ()};
}

Vec3 RigidBody::getRotation() const
{
	JPH::Vec3 rotation = m_bodyInterface.GetRotation(m_body).GetEulerAngles();
	return Vec3{rotation.GetX(), rotation.GetY(), rotation.GetZ()};
}

Vec3 RigidBody::getLinearVelocity() const
{
	JPH::Vec3 linearVelocity = m_bodyInterface.GetLinearVelocity(m_body);
	return Vec3{linearVelocity.GetX(), linearVelocity.GetY(), linearVelocity.GetZ()};
}

void RigidBody::setPosition(const Vec3& position)
{
	m_bodyInterface.SetPosition(m_body, JPH::Vec3(position.x, position.y, position.z), JPH::EActivation::DontActivate);
}

void RigidBody::setLinearVelocity(const Vec3& velocity)
{
	m_bodyInterface.SetLinearVelocity(m_body, JPH::Vec3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::addForce(const Vec3& force)
{
	m_bodyInterface.AddForce(m_body, JPH::Vec3(force.x, force.y, force.z));
}

void RigidBody::addImpulse(const Vec3& impulse)
{
	m_bodyInterface.AddImpulse(m_body, JPH::Vec3(impulse.x, impulse.y, impulse.z));
}

void RigidBody::activate()
{
	m_bodyInterface.ActivateBody(m_body);
}