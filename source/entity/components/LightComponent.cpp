#include <entity/components/LightComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/Entity.h>

using namespace genesis;

LightComponent::LightComponent(const ComponentDesc& desc):
	Component(desc),
	m_type{LightType::Point},
	m_color{1.0f, 1.0f, 1.0f},
	m_intensity{1.0f},
	m_radius{5.0f},
	m_enabled{true}
{
	if (!m_entity.hasComponent<TransformComponent>()) {
		m_entity.createComponent<TransformComponent>();
	}
}

LightComponent::~LightComponent() {}

LightComponent::LightType LightComponent::getType() const noexcept
{
	return m_type;
}

Vec3 LightComponent::getColor() const noexcept
{
	return m_color;
}

float LightComponent::getIntensity() const noexcept
{
	return m_intensity;
}

float LightComponent::getRadius() const noexcept
{
	return m_radius;
}

bool LightComponent::isEnabled() const noexcept
{
	return m_enabled;
}

void LightComponent::setType(LightType type) noexcept
{
	m_type = type;
}

void LightComponent::setColor(const Vec3& color) noexcept
{
	m_color = color;
}

void LightComponent::setIntensity(float intensity) noexcept
{
	m_intensity = intensity;
}

void LightComponent::setRadius(float radius) noexcept
{
	m_radius = radius;
}

void LightComponent::setEnabled(bool enabled) noexcept
{
	m_enabled = enabled;
}