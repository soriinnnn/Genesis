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
    return m_body.get();
}

void RigidBodyComponent::setBody(const SharedPtr<RigidBody>& body)
{
    m_body = body;
}