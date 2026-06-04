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

	struct TextureFilterUIOption
	{
		const char* id;
		Point margin;
		TextureFiltering filter;
		const char* labelContent;
		Rect labelSize;
	};

	struct AntiAliasingUIOption
	{
		const char* id;
		Point margin;
		AntiAliasing antialiasing;
		const char* labelContent;
		Rect labelSize;
	};
}

#endif