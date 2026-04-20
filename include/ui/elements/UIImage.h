#ifndef GENESIS_UI_IMAGE_H
#define GENESIS_UI_IMAGE_H
#include <ui/elements/UIElement.h>

namespace genesis
{
	class UIImage final: public UIElement
	{
	public:
		explicit UIImage(const UIElementDesc& desc);
		~UIImage() override;

		void render(SpriteBatch& batch) override;

	private:

	};
}

#endif