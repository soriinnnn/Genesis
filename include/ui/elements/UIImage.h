#ifndef GENESIS_UI_IMAGE_H
#define GENESIS_UI_IMAGE_H
#include <ui/elements/UIElement.h>
#include <math/Vec2.h>

namespace genesis
{
	class UIImage final: public UIElement
	{
	public:
		explicit UIImage(const UIElementDesc& desc);
		~UIImage() override;

		void render(SpriteBatch& batch) override;

		void setTexture(SharedPtr<Texture> texture) noexcept;

	private:
		SharedPtr<Texture> m_texture;
	};
}

#endif