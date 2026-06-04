#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Macros.h"
#include "Types.h"
#include <core/Core.h>
#include <math/Rect.h>
#include <math/Vec4.h>

namespace constants
{
	using namespace genesis;

	// USER INTERFACE
	const Rect refCanvasSize = Rect{TARGET_RESOLUTION_WIDTH, TARGET_RESOLUTION_HEIGHT};

	const Vec4 fontColor = Vec4{1.0f, 1.0f, 1.0f, 1.0f};
	const Rect buttonSize = Rect{150, 50};
	const Vec4 buttonIdleColor = Vec4{0.1f, 0.1f, 0.1f, 1.0f};
	const Vec4 buttonHoverColor = Vec4{0.3f, 0.3f, 0.3f, 1.0f};
	const Vec4 buttonSelectedIdleColor = Vec4{0.1f, 0.15f, 0.3f, 1.0f};
	const Vec4 buttonSelectedHoverColor = Vec4{0.3f, 0.35f, 0.5f, 1.0f};

	const String mainMenuElements[] = {
		UI_MAIN_MENU_PANEL,
		UI_MAIN_MENU_TITLE,
		UI_MAIN_MENU_START_BUTTON,
		UI_MAIN_MENU_QUIT_BUTTON,
		UI_MAIN_MENU_SETTINGS_BUTTON
	};

	const String settingsMenuElements[] = {
		UI_SETTINGS_MENU_PANEL,
		UI_SETTINGS_MENU_TITLE,
		UI_SETTINGS_MENU_BACK_BUTTON,
		UI_SETTINGS_MENU_TEX_FILTERING_LABEL,
		UI_SETTINGS_MENU_TEX_FILTERING_OPTION1,
		UI_SETTINGS_MENU_TEX_FILTERING_OPTION2,
		UI_SETTINGS_MENU_TEX_FILTERING_OPTION3,
		UI_SETTINGS_MENU_TEX_FILTERING_OPTION4,
		UI_SETTINGS_MENU_ANTIALIASING_LABEL,
		UI_SETTINGS_MENU_ANTIALIASING_OPTION1,
		UI_SETTINGS_MENU_ANTIALIASING_OPTION2,
		UI_SETTINGS_MENU_ANTIALIASING_OPTION3,
		UI_SETTINGS_MENU_ANTIALIASING_OPTION4
	};

	const String mainMenuHints[] = {
		UI_HINT1,
		UI_HINT2,
		UI_HINT3,
		UI_HINT4
	};
 
	const types::TextureFilterUIOption textureFilterOptions[] = {
		{UI_SETTINGS_MENU_TEX_FILTERING_OPTION1, {-188, -93}, TextureFiltering::Trilinear, "Trilinear", {80, 22}},
		{UI_SETTINGS_MENU_TEX_FILTERING_OPTION2, {0, -93}, TextureFiltering::Anisotropic_4X, "Anisotropic 4x", {138, 22}},
		{UI_SETTINGS_MENU_TEX_FILTERING_OPTION3, {188, -93}, TextureFiltering::Anisotropic_8X, "Anisotropic 8x", {138, 22}},
		{UI_SETTINGS_MENU_TEX_FILTERING_OPTION4, {-188, -23}, TextureFiltering::Anisotropic_16X,"Anisotropic 16x", {150, 22}}
	};

	const types::AntiAliasingUIOption antiAliasingOptions[] = {
		{UI_SETTINGS_MENU_ANTIALIASING_OPTION1, {-188, 106}, AntiAliasing::None, "Disabled", {88, 22}},
		{UI_SETTINGS_MENU_ANTIALIASING_OPTION2, {0, 106}, AntiAliasing::MSAA_2X, "MSAA 2x", {92, 22}},
		{UI_SETTINGS_MENU_ANTIALIASING_OPTION3, {188, 106}, AntiAliasing::MSAA_4X, "MSAA 4x", {92, 22}},
		{UI_SETTINGS_MENU_ANTIALIASING_OPTION4, {-188, 176}, AntiAliasing::MSAA_8X, "MSAA 8x", {92, 22}}
	};
}

#endif