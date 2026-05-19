#include <entity/components/RigidBodyComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/Entity.h>
#include <physics/RigidBody.h>

using namespace genesis;

RigidBodyComponent::RigidBodyComponent(const ComponentDesc& desc): Component(desc) 
{
    if (!m_entity.hasComponent<TransformComponent>()) {
        m_entity.createComponent<TransformComponent>();
    }
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
        m_body->m_entity = nullptr;
    }
    m_body = body;
    m_body->m_entity = &m_entity;
}