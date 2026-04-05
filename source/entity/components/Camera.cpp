#include <entity/components/Camera.h>
#include <entity/components/Transform.h>
#include <entity/Entity.h>
#include <core/utils/Macros.h>

using namespace genesis;

static Vec3 getForwardVector(const Vec3& rotation);

Camera::Camera(const ComponentDesc& desc): Component(desc), 
    m_fov{1.047f},
    m_nearPlane{0.1f},
    m_farPlane{1000.0f},
    m_aspectRatio{16.0f/9.0f},
    m_isProjectionDirty{true}
{
    m_entity.createComponent<Transform>();
}

Camera::~Camera() {}

float Camera::getFov() const noexcept
{
    return m_fov;
}

float Camera::getNearPlane() const noexcept
{
    return m_nearPlane;
}

float Camera::getFarPlane() const noexcept
{
    return m_farPlane;
}

float Camera::getAspectRatio() const noexcept
{
    return m_aspectRatio;
}

void Camera::setFov(float fov) noexcept
{
    m_fov = fov;
    m_isProjectionDirty = true;
}

void Camera::setNearPlane(float nearPlane) noexcept
{
    m_nearPlane = nearPlane;
    m_isProjectionDirty = true;
}

void Camera::setFarPlane(float farPlane) noexcept
{
    m_farPlane = farPlane;
    m_isProjectionDirty = true;
}

void Camera::setAspectRatio(float aspectRatio) noexcept
{
    m_aspectRatio = aspectRatio;
    m_isProjectionDirty = true;
}

const Mat4& Camera::getProjectionMatrix() noexcept
{
    if (m_isProjectionDirty) {
        updateProjectionMatrix();
        m_isProjectionDirty = false;
    }
    return m_projection;
}

Mat4 Camera::getViewMatrix() noexcept
{
    Transform* transform = m_entity.getComponent<Transform>();
    return Mat4::lookAtLH(
        transform->getPosition(),
        transform->getPosition() + getForwardVector(transform->getRotation()),
        Vec3{0.0f, 1.0f, 0.0f}
    );
}

void Camera::updateProjectionMatrix() noexcept
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