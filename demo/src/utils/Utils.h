#ifndef UTILS_H
#define UTILS_H
#include "Constants.h"
#include "Types.h"

#include <game/Game.h>
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

	inline void setUIVisibility(bool visible, const Vector<String>& elements, UIManager& manager)
	{
		for (const String& element : elements) {
			UIElement* elem = manager.getElement<UIElement>(element.c_str());
			if (elem) {
				elem->setVisible(visible);
			}
		}
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

	// pre: t >= 0 && t <= 1.0f
	inline float applyEasing(Easing easing, float t)
	{
		switch (easing) {
		case Easing::Linear:
			return t;
		case Easing::EaseInQuad:
			return t * t;
		case Easing::EaseOutQuad:
			return t * (2.0f - t);
		case Easing::EaseInOutQuad:
			return (t < 0.5f) ? 2.0f * t * t : -1.0f + t * (4.0f - 2.0f * t);
		case Easing::EaseInCubic:
			return t * t * t;
		case Easing::EaseOutCubic:
			return t * (3.0f + t * (-3.0f + t));
		case Easing::EaseInOutCubic:
			return (t < 0.5f) ? 4.0f * t * t * t : -3.0f + t * (12.0f + t * (-12.0f + 4.0f * t));
		case Easing::EaseInSine:
			return 1.0f - std::cos((pi * t) / 2.0f);
		case Easing::EaseOutSine:
			return std::sin((pi * t) / 2.0f);
		case Easing::EaseInOutSine:
			return -(std::cos(pi * t) - 1.0f) / 2.0f;
		default:
			return t;
		}
	}

	// pre: t >= 0 && t <= 1.0f
	inline void updateAnimation(Animation& anim, float deltaTime)
	{
		uint32 transitionCount = static_cast<uint32>(anim.transitions.size());
		if (anim.currentTransition >= transitionCount) {
			return;
		}

		const Transition& transition = anim.transitions[anim.currentTransition];
		anim.currentTime += deltaTime;

		float t = anim.currentTime / transition.duration;
		if (t < 1.0f) {
			anim.target = Vec3::lerp(transition.origin, transition.target, applyEasing(transition.easing, t));
			return;
		}

		anim.target = transition.target;
		anim.currentTransition++;
		anim.currentTime = 0.0f;

		if (anim.loop && anim.currentTransition >= transitionCount) {
			anim.target = anim.transitions[0].origin;
			anim.currentTransition = 0;
		}
	}

	inline Vector<Transition> offsetTransitions(const Transition* transitions, uint32 count, const Vec3& offset) {
		Vector<Transition> result(count);

		for (uint32 i = 0; i < count; i++) {
			result[i].origin = transitions[i].origin + offset;
			result[i].target = transitions[i].target + offset;
			result[i].duration = transitions[i].duration;
		}

		return result;
	}

	// for arrays
	template<typename T, size_t N>
	constexpr uint32 countof(T(&)[N])
	{
		return static_cast<uint32>(N);
	}

	// math

	inline void clamp(float& value, float min, float max)
	{
		if (value > max) {
			value = max;
		}
		else if (value < min) {
			value = min;
		}
	}
	
	static std::mt19937 rng(std::random_device{}());

	inline float range(float min, float max)
	{
		return std::uniform_real_distribution(min, max)(rng);
	}

	inline int range(int min, int max)
	{
		return std::uniform_int_distribution(min, max)(rng);
	}

	inline Vec3 getRandomVector3(Vec3 bounds)
	{
		Vec3 half = bounds / 2.0f;
		return Vec3{
			range(-half.x, half.x),
			range(-half.y, half.y),
			range(-half.z, half.z)
		};
	}

	inline Vec3 getRandomVector3(float radius)
	{
		Vec3 position;
		do {
			position = Vec3{
				range(-radius, radius),
				range(-radius, radius),
				range(-radius, radius)
			};
		} 
		while (Vec3::norm(position) > radius);
		return position;
	}
}

#endif