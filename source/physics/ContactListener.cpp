#include <physics/ContactListener.h>

using namespace genesis;

static Vec3 toGenesisVec3(const JPH::Vec3& v);

ContactListener::ContactListener(const ContactListenerDesc& desc): JPH::ContactListener(), m_bodyInterface{desc.bodyInterface} {}

JPH::ValidateResult ContactListener::OnContactValidate(const JPH::Body& body1, const JPH::Body& body2, JPH::RVec3Arg baseOffset, const JPH::CollideShapeResult& collisionResult)
{
	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListener::OnContactAdded(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings)
{
	RigidBody* rb1 = reinterpret_cast<RigidBody*>(body1.GetUserData());
	RigidBody* rb2 = reinterpret_cast<RigidBody*>(body2.GetUserData());

	ContactEvent event{};
	event.type = EventType::Added;
	event.added.penetrationDepth = manifold.mPenetrationDepth;
	
	std::lock_guard lock(m_mutex);

	event.body = rb1;
	event.added.other = rb2;
	event.added.contactPoint = toGenesisVec3(manifold.GetWorldSpaceContactPointOn1(0));
	event.added.contactNormal = toGenesisVec3(manifold.mWorldSpaceNormal);
	m_events.push_back(event);

	event.body = rb2;
	event.added.other = rb1;
	event.added.contactPoint = toGenesisVec3(manifold.GetWorldSpaceContactPointOn2(0));
	event.added.contactNormal = -event.added.contactNormal;
	m_events.push_back(event);
}

void ContactListener::OnContactPersisted(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold& manifold, JPH::ContactSettings& settings)
{
	RigidBody* rb1 = reinterpret_cast<RigidBody*>(body1.GetUserData());
	RigidBody* rb2 = reinterpret_cast<RigidBody*>(body2.GetUserData());

	ContactEvent event{};
	event.type = EventType::Persisted;
	event.persisted.penetrationDepth = manifold.mPenetrationDepth;

	std::lock_guard lock(m_mutex);

	event.body = rb1;
	event.persisted.other = rb2;
	event.persisted.contactPoint = toGenesisVec3(manifold.GetWorldSpaceContactPointOn1(0));
	event.persisted.contactNormal = toGenesisVec3(manifold.mWorldSpaceNormal);
	m_events.push_back(event);

	event.body = rb2;
	event.persisted.other = rb1;
	event.persisted.contactPoint = toGenesisVec3(manifold.GetWorldSpaceContactPointOn2(0));
	event.persisted.contactNormal = -event.persisted.contactNormal;
	m_events.push_back(event);
}

void ContactListener::OnContactRemoved(const JPH::SubShapeIDPair& subShapePair) 
{
	std::lock_guard lock(m_mutex);
	m_removedContacts.push_back(subShapePair);
}

void ContactListener::dispatchEvents()
{
	for (auto& contact : m_removedContacts) {
		RigidBody* rb1 = reinterpret_cast<RigidBody*>(m_bodyInterface.GetUserData(contact.GetBody1ID()));
		RigidBody* rb2 = reinterpret_cast<RigidBody*>(m_bodyInterface.GetUserData(contact.GetBody2ID()));

		ContactEvent event{};
		event.type = EventType::Removed;

		if (rb1) {
			event.body = rb1;
			event.removed.other = rb2;
			m_events.push_back(event);
		}

		if (rb2) {
			event.body = rb2;
			event.removed.other = rb1;
			m_events.push_back(event);
		}
	}
	m_removedContacts.clear();

	for (auto& event : m_events) {
		switch (event.type) {
			case EventType::Added: {
				if (event.body->m_onContactAdded) {
					event.body->m_onContactAdded(event.added);
				}
				break;
			}
			case EventType::Persisted: {
				if (event.body->m_onContactPersisted) {
					event.body->m_onContactPersisted(event.persisted);
				}
				break;
			}
			case EventType::Removed: {
				if (event.body->m_onContactRemoved) {
					event.body->m_onContactRemoved(event.removed);
				}
			}
		}
	}
	m_events.clear();
}

/* STATIC FUNCTIONS DEFINITIONS */

Vec3 toGenesisVec3(const JPH::Vec3& v)
{
	return Vec3{v.GetX(), v.GetY(), v.GetZ()};
}
