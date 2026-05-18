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

		void setTexture(const SharedPtr<Texture>& texture) noexcept;
		void setAutoSize(bool enable) noexcept;

	private:
		void applyAutoSize();

		void onRender(SpriteBatch& batch) override;
		void onSize() override;

	private:
		SharedPtr<Texture> m_texture;
		bool m_autoSize;
	};
}

#endif