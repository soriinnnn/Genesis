#ifndef GENESIS_UI_PANEL_H
#define GENESIS_UI_PANEL_H
#include <ui/elements/UIElement.h>

namespace genesis
{
	class UIPanel final: public UIElement
	{
	public:
		explicit UIPanel(const UIElementDesc& desc);
		~UIPanel() override;

	private:
		void onRender(SpriteBatch& batch) override;
	};
}

#endif