#include <entity/components/CameraComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/Entity.h>
#include <core/utils/Macros.h>

using namespace genesis;

static Vec3 getForwardVector(const Vec3& rotation);

CameraComponent::CameraComponent(const ComponentDesc& desc): Component(desc),
    m_fov{1.047f},
    m_nearPlane{0.1f},
    m_farPlane{1000.0f},
    m_aspectRatio{16.0f/9.0f},
    m_isDirty{true}
{
    if (!m_entity.hasComponent<TransformComponent>()) {
        m_entity.createComponent<TransformComponent>();
    }
}

CameraComponent::~CameraComponent() {}

float CameraComponent::getFov() const noexcept
{
    return m_fov;
}

float CameraComponent::getNearPlane() const noexcept
{
    return m_nearPlane;
}

float CameraComponent::getFarPlane() const noexcept
{
    return m_farPlane;
}

float CameraComponent::getAspectRatio() const noexcept
{
    return m_aspectRatio;
}

void CameraComponent::setFov(float fov) noexcept
{
    m_fov = fov;
    m_isDirty = true;
}

void CameraComponent::setNearPlane(float nearPlane) noexcept
{
    m_nearPlane = nearPlane;
    m_isDirty = true;
}

void CameraComponent::setFarPlane(float farPlane) noexcept
{
    m_farPlane = farPlane;
    m_isDirty = true;
}

void CameraComponent::setAspectRatio(float aspectRatio) noexcept
{
    m_aspectRatio = aspectRatio;
    m_isDirty = true;
}

const Mat4& CameraComponent::getProjectionMatrix() noexcept
{
    if (m_isDirty) {
        updateProjectionMatrix();
        m_isDirty = false;
    }
    return m_projection;
}

Mat4 CameraComponent::getViewMatrix() noexcept
{
    TransformComponent* transform = m_entity.getComponent<TransformComponent>();
    return Mat4::lookAtLH(
        transform->getPosition(),
        transform->getPosition() + getForwardVector(transform->getRotation()),
        Vec3{0.0f, 1.0f, 0.0f}
    );
}

void CameraComponent::updateProjectionMatrix() noexcept
{
    m_projection = Mat4::perspectiveLH(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

/* STATIC FUNCTION DEFINITIONS */

static Vec3 getForwardVector(const Vec3& rotation)
{
    return Vec3{
         std::cos(rotation.x) * std::sin(rotation.y),
        -std::sin(rotation.x),
         std::cos(rotation.x) * std::cos(rotation.y)
    };
}