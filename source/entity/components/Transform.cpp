#include <entity/components/Transform.h>
#include <cmath>

using namespace genesis;
using namespace std;

Transform::Transform(const ComponentDesc& desc): 
	Component(desc), 
	m_position{0.0f, 0.0f, 0.0f}, 
	m_rotation{0.0f, 0.0f, 0.0f}, 
	m_scale{1.0f, 1.0f, 1.0f},
	m_isDirty{true}
{}

Transform::~Transform() {}

Vec3 Transform::getPosition() const noexcept
{
	return m_position;
}

Vec3 Transform::getRotation() const noexcept
{
	return m_rotation;
}

Vec3 Transform::getScale() const noexcept
{
	return m_scale;
}

const Mat4& Transform::getWorldMatrix() noexcept
{
	if (m_isDirty) {
		updateWorldMatrix();
		m_isDirty = false;
	}
	return m_world;
}

Vec3 Transform::getForwardVector() const noexcept
{
	return Vec3{
		 cos(m_rotation.x) * sin(m_rotation.y),
		-sin(m_rotation.x),
		 cos(m_rotation.x) * cos(m_rotation.y)
	};
}

void Transform::setPosition(const Vec3& position) noexcept
{
	m_position = position;
	m_isDirty = true;
}

void Transform::setRotation(const Vec3& rotation) noexcept
{
	m_rotation = rotation;
	m_isDirty = true;
}

void Transform::setScale(const Vec3& scale) noexcept
{
	m_scale = scale;
	m_isDirty = true;
}

void Transform::updateWorldMatrix() noexcept
{
	m_world = Mat4::fromScale(m_scale) * Mat4::fromRotation(m_rotation) * Mat4::fromTranslation(m_position);
}