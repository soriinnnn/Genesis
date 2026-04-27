#ifndef GENESIS_LIGHT_COMPONENT_H
#define GENESIS_LIGHT_COMPONENT_H
#include <entity/components/Component.h>
#include <math/Vec3.h>

namespace genesis
{
	class LightComponent final: public Component
	{
	public:
		enum class LightType
		{
			Directional,
			Point
		};

	public:
		explicit LightComponent(const ComponentDesc& desc);
		virtual ~LightComponent() override;

		LightType getType() const noexcept;
		Vec3 getColor() const noexcept;
		float getIntensity() const noexcept;
		float getRadius() const noexcept;
		bool isEnabled() const noexcept;

		void setType(LightType type) noexcept;
		void setColor(const Vec3& color) noexcept;
		void setIntensity(float intensity) noexcept;
		void setRadius(float radius) noexcept;
		void setEnabled(bool enabled) noexcept;

	private:
		LightType m_type;
		Vec3 m_color;
		float m_intensity;
		float m_radius;
		bool m_enabled;
	};
}

#endif