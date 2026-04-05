#ifndef GENESIS_TRANSFORM_H
#define GENESIS_TRANSFORM_H
#include <entity/components/Component.h>
#include <math/Vec3.h>
#include <math/Mat4.h>

namespace genesis
{
	class Transform final: public Component
	{
	public:
		explicit Transform(const ComponentDesc& desc);
		~Transform() override;

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
}

#endif