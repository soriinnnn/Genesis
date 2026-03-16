#include <entity/components/Component.h>	

using namespace genesis;

Component::Component(const ComponentDesc& desc): m_entity{desc.entity} {}

Component::~Component() {}