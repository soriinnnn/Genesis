#include <entity/components/Component.h>	

using namespace genesis;

Component::Component(const ComponentDesc& desc): Base(desc.base), m_entity{desc.entity} {}

Component::~Component() {}

void Component::update(float deltaTime) {}

Entity& Component::getEntity()
{
    return m_entity;
}
