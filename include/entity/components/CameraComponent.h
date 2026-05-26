#ifndef GENESIS_CAMERA_H
#define GENESIS_CAMERA_H
#include <entity/components/Component.h>
#include <math/Mat4.h>

GENESIS_NAMESPACE_START

class CameraComponent final: public Component
{
public:
    explicit CameraComponent(const ComponentDesc& desc);
    ~CameraComponent() override;

    float getFov() const noexcept;
    float getNearPlane() const noexcept;
    float getFarPlane() const noexcept;
    float getAspectRatio() const noexcept;

    void setFov(float fov) noexcept;
    void setNearPlane(float nearPlane) noexcept;
    void setFarPlane(float farPlane) noexcept;
    void setAspectRatio(float aspectRatio) noexcept;
    const Mat4& getProjectionMatrix() noexcept;
    Mat4 getViewMatrix() noexcept;

private:
    void updateProjectionMatrix() noexcept;

private:
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
    float m_aspectRatio;
    Mat4 m_projection;
    bool m_isDirty;
};

GENESIS_NAMESPACE_END

#endif