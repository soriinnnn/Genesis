#ifndef GENESIS_TRANSFORM_COMPONENT_H
#define GENESIS_TRANSFORM_COMPONENT_H
#include <entity/components/Component.h>
#include <math/Vec3.h>
#include <math/Mat4.h>

GENESIS_NAMESPACE_START

class TransformComponent final: public Component
{
public:
	explicit TransformComponent(const ComponentDesc& desc);
	~TransformComponent() override;

	Vec3 getPosition() const noexcept;
	Vec3 getRotation() const noexcept;
	Vec3 getScale() const noexcept;
	const Mat4& getWorldMatrix() noexcept;
	Vec3 getForwardVector() const noexcept;

	void setPosition(const Vec3& position) noexcept;
	void setRotation(const Vec3& rotation) noexcept;
	void setScale(const Vec3& scale) noexcept;

private:
	void updateWorldMatrix() noexcept;

private:
	Vec3 m_position;
	Vec3 m_rotation;
	Vec3 m_scale;
	Mat4 m_world;
	bool m_isDirty;
};

GENESIS_NAMESPACE_END

#endif