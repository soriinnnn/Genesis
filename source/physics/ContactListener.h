#ifndef GENESIS_CONTACT_LISTENER_H
#define GENESIS_CONTACT_LISTENER_H
#include <jolt/Jolt.h>
#include <jolt/Physics/Collision/ContactListener.h>
#include <physics/RigidBody.h>
#include <mutex>

GENESIS_NAMESPACE_BEGIN

struct ContactListenerDesc
{
	JPH::BodyInterface& bodyInterface;
};

class ContactListener final: public JPH::ContactListener
{
public:
	explicit ContactListener(const ContactListenerDesc& desc);

	JPH::ValidateResult OnContactValidate(const JPH::Body& body1, const JPH::Body& body2, JPH::RVec3Arg baseOffset, const JPH::CollideShapeResult& collisionResult) override;
	void OnContactAdded(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings) override;
	void OnContactPersisted(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings) override;
	void OnContactRemoved(const JPH::SubShapeIDPair& subShapePair) override;

	void dispatchEvents();

private:
	enum class EventType
	{
		Added,
		Persisted,
		Removed
	};

	struct ContactEvent
	{
		EventType type;
		RigidBody* body;
		union {
			RigidBody::ContactAddedData added;
			RigidBody::ContactPersistedData persisted;
			RigidBody::ContactRemovedData removed;
		};
	};

private:
	JPH::BodyInterface& m_bodyInterface;
	Vector<JPH::SubShapeIDPair> m_removedContacts;
	Vector<ContactEvent> m_events;
	std::mutex m_mutex;
};

GENESIS_NAMESPACE_END

#endif