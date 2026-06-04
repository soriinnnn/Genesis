#ifndef UTILS_H
#define UTILS_H
#include "Constants.h"
#include "Types.h"

#include <ui/elements/UIImage.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIButton.h>
#include <math/Point.h>
#include <math/Rect.h>
#include <math/Vec2.h>

namespace utils
{
	using namespace genesis;
	using namespace constants;
	using namespace types;

	// pre: referenceSize.width() > 0, referenceSize.height() > 0
	inline Vec2 getSizeScaleRatio(Rect currentSize, Rect referenceSize)
	{
		return Vec2{
			static_cast<float>(currentSize.width()) / referenceSize.width(),
			static_cast<float>(currentSize.height()) / referenceSize.height()
		};
	}

	inline Point applyMarginScale(Point baseMargin, Vec2 scaleRatio)
	{
		return Point{
			static_cast<int32>(std::round(baseMargin.x * scaleRatio.x)),
			static_cast<int32>(std::round(baseMargin.y * scaleRatio.y))
		};
	}

	inline UIImage* createUIImage(Game& game, UIImageCreateInfo info)
	{
		GameContext context = game.getContext();
		UIImage* image = context.ui.createElement<UIImage>(info.id);

		image->setTexture(context.resources.getTexture(info.texture));
		image->setColor(info.color);
		image->setAnchor(info.anchor);
		image->setMargin(info.margin);
		image->setVisible(false);

		return image;
	}

	inline UILabel* createUILabel(Game& game, UILabelCreateInfo info)
	{
		GameContext context = game.getContext();
		UILabel* label = context.ui.createElement<UILabel>(info.id);

		label->setFont(context.resources.getFont(info.font));
		label->setContent(info.content);
		label->setColor(fontColor);
		label->setSize(info.size);
		label->setAnchor(info.anchor);
		label->setMargin(info.margin);
		label->setVisible(false);

		return label;
	}

	inline UIButton* createUIButton(Game& game, UIButtonCreateInfo info)
	{
		GameContext context = game.getContext();
		UIButton* button = context.ui.createElement<UIButton>(info.id);
		UILabel* label = button->getLabel();

		button->setColor(buttonIdleColor);
		button->setSize(buttonSize);
		button->setAnchor(info.anchor);
		button->setMargin(info.margin);
		button->setVisible(false);

		label->setFont(context.resources.getFont(info.labelFont));
		label->setContent(info.labelContent);
		label->setColor(fontColor);
		label->setSize(info.labelSize);
		label->setAnchor(Anchor::Center);

		return button;
	}

	inline void updateFPSLabel(float deltaTime, UILabel& label)
	{
		static float timeElapsed = 0.0f;
		static int frameCount = 0;

		timeElapsed += deltaTime;
		frameCount++;

		if (timeElapsed >= 1.0f) {
			int fps = static_cast<int>(std::round(frameCount / timeElapsed));

			String content = "FPS: " + std::to_string(fps);
			label.setContent(content.c_str());

			timeElapsed = 0.0f;
			frameCount = 0;
		}
	}
}

#endif