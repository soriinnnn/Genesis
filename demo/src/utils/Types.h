#ifndef TYPES_H
#define TYPES_H
#include <core/Core.h>
#include <core/utils/Types.h>
#include <ui/elements/UIElement.h>
#include <math/Point.h>
#include <math/Rect.h>
#include <math/Vec4.h>

namespace types
{
	using namespace genesis;

	struct UIImageCreateInfo
	{
		const char* id;
		const char* texture;
		Vec4 color;
		Anchor anchor;
		Point margin;
	};

	struct UILabelCreateInfo
	{
		const char* id;
		const char* font;
		const char* content;
		Rect size;
		Anchor anchor;
		Point margin;
	};

	struct UIButtonCreateInfo
	{
		const char* id;
		Anchor anchor;
		Point margin;
		const char* labelFont;
		const char* labelContent;
		Rect labelSize;
	};

	struct TextureFilterOption
	{
		UIButtonCreateInfo buttonInfo;
		TextureFiltering textureFilter;
	};

	struct AntiAliasingOption
	{
		UIButtonCreateInfo buttonInfo;
		AntiAliasing antiAliasing;
	};
	
	enum class Easing
	{
		Linear,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
		EaseInCubic,
		EaseOutCubic,
		EaseInOutCubic,
		EaseInSine,
		EaseOutSine,
		EaseInOutSine
	};

	struct Transition
	{
		Vec3 origin;
		Vec3 target;
		float duration;
		Easing easing = Easing::Linear;
	};

	struct Animation
	{
		Vec3 target;
		Vector<Transition> transitions;
		uint32 currentTransition = 0;
		float currentTime = 0.0f;
		bool loop = true;
	};
}

#endif