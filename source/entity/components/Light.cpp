#include <entity/components/Light.h>
#include <entity/components/Transform.h>
#include <entity/Entity.h>

using namespace genesis;

Light::Light(const ComponentDesc& desc):
	Component(desc),
	m_type{LightType::Point},
	m_color{1.0f, 1.0f, 1.0f},
	m_intensity{1.0f},
	m_radius{5.0f},
	m_enabled{true}
{
	m_entity.createComponent<Transform>();
}

Light::~Light() {}

Light::LightType Light::getType() const noexcept
{
	return m_type;
}

Vec3 Light::getColor() const noexcept
{
	return m_color;
}

float Light::getIntensity() const noexcept
{
	return m_intensity;
}

float Light::getRadius() const noexcept
{
	return m_radius;
}

bool Light::isEnabled() const noexcept
{
	return m_enabled;
}

void Light::setType(LightType type) noexcept
{
	m_type = type;
}

void Light::setColor(const Vec3& color) noexcept
{
	m_color = color;
}

void Light::setIntensity(float intensity) noexcept
{
	m_intensity = intensity;
}

void Light::setRadius(float radius) noexcept
{
	m_radius = radius;
}

void Light::setEnabled(bool enabled) noexcept
{
	m_enabled = enabled;
}