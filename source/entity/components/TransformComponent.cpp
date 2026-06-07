#include <entity/components/TransformComponent.h>
#include <cmath>

using namespace genesis;
using namespace std;

TransformComponent::TransformComponent(const ComponentDesc& desc):
	Component(desc), 
	m_position{0.0f, 0.0f, 0.0f}, 
	m_rotation{0.0f, 0.0f, 0.0f}, 
	m_scale{1.0f, 1.0f, 1.0f},
	m_isDirty{true}
{}

TransformComponent::~TransformComponent() {}

Vec3 TransformComponent::getPosition() const noexcept
{
	return m_position;
}

Vec3 TransformComponent::getRotation() const noexcept
{
	return m_rotation;
}

Vec3 TransformComponent::getScale() const noexcept
{
	return m_scale;
}

const Mat4& TransformComponent::getWorldMatrix() noexcept
{
	if (m_isDirty) {
		updateWorldMatrix();
		m_isDirty = false;
	}
	return m_world;
}

Vec3 TransformComponent::getForwardVector() const noexcept
{
	float cosX = cosf(m_rotation.x);
	float cosY = cosf(m_rotation.y);
	float cosZ = cosf(m_rotation.z);
	float sinX = sinf(m_rotation.x);
	float sinY = sinf(m_rotation.y);
	float sinZ = sinf(m_rotation.z);

	return Vec3{
		 cosX * sinY * cosZ + sinX * sinZ,
		 cosX * sinY * sinZ - sinX * cosZ,
		 cosX * cosY
	};
}

Vec3 TransformComponent::getRightVector() const noexcept
{
	float cosY = cosf(m_rotation.y);
	float cosZ = cosf(m_rotation.z);
	float sinY = sinf(m_rotation.y);
	float sinZ = sinf(m_rotation.z);

	return Vec3{
		cosY * cosZ,
		cosY * sinZ,
		-sinY
	};
}

Vec3 TransformComponent::getUpVector() const noexcept
{
	float cosX = cosf(m_rotation.x);
	float cosY = cosf(m_rotation.y);
	float cosZ = cosf(m_rotation.z);
	float sinX = sinf(m_rotation.x);
	float sinY = sinf(m_rotation.y);
	float sinZ = sinf(m_rotation.z);

	return Vec3{
		 sinX * sinY * cosZ - cosX * sinZ,
		 sinX * sinY * sinZ + cosX * cosZ,
		 sinX * cosY
	};
}

void TransformComponent::setPosition(const Vec3& position) noexcept
{
	m_position = position;
	m_isDirty = true;
}

void TransformComponent::setRotation(const Vec3& rotation) noexcept
{
	m_rotation = rotation;
	m_isDirty = true;
}

void TransformComponent::setScale(const Vec3& scale) noexcept
{
	m_scale = scale;
	m_isDirty = true;
}

void TransformComponent::updateWorldMatrix() noexcept
{
	m_world = Mat4::fromScale(m_scale) * Mat4::fromRotation(m_rotation) * Mat4::fromTranslation(m_position);
}