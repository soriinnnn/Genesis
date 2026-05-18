#include <entity/components/RigidBodyComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/Entity.h>
#include <physics/RigidBody.h>

using namespace genesis;

RigidBodyComponent::RigidBodyComponent(const ComponentDesc& desc): Component(desc) 
{
    m_entity.createComponent<TransformComponent>();
}

RigidBodyComponent::~RigidBodyComponent() {}

RigidBody* RigidBodyComponent::getBody()
{
    GENESIS_ASSERT(m_body.get() != nullptr, "Body is null.");
    return m_body.get();
}

void RigidBodyComponent::setBody(const SharedPtr<RigidBody>& body)
{
    if (!body) {
        return;
    }
    if (m_body) {
        m_body->setEntity(nullptr);
    }
    m_body = body;
    m_body->setEntity(&m_entity);
}