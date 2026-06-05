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
	using namespace types;

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
 
	const TextureFilterOption textureFilterOptions[] = {
		{{UI_SETTINGS_MENU_TEX_FILTERING_OPTION1, Anchor::Center, {-188, -93}, ASSETS_FONT_PRIMARY_16_PX, "Trilinear", {80, 22}}, TextureFiltering::Trilinear},
		{{UI_SETTINGS_MENU_TEX_FILTERING_OPTION2, Anchor::Center, {0, -93}, ASSETS_FONT_PRIMARY_16_PX, "Anisotropic 4x", {138, 22}}, TextureFiltering::Anisotropic_4X},
		{{UI_SETTINGS_MENU_TEX_FILTERING_OPTION3, Anchor::Center, {188, -93}, ASSETS_FONT_PRIMARY_16_PX, "Anisotropic 8x", {138, 22}}, TextureFiltering::Anisotropic_8X},
		{{UI_SETTINGS_MENU_TEX_FILTERING_OPTION4, Anchor::Center, {-188, -23}, ASSETS_FONT_PRIMARY_16_PX, "Anisotropic 16x", {150, 22}}, TextureFiltering::Anisotropic_16X}
	};

	const AntiAliasingOption antiAliasingOptions[] = {
		{{UI_SETTINGS_MENU_ANTIALIASING_OPTION1, Anchor::Center, {-188, 106}, ASSETS_FONT_PRIMARY_16_PX, "Disabled", {88, 22}}, AntiAliasing::None},
		{{UI_SETTINGS_MENU_ANTIALIASING_OPTION2, Anchor::Center, {0, 106}, ASSETS_FONT_PRIMARY_16_PX, "MSAA 2x", {92, 22}}, AntiAliasing::MSAA_2X},
		{{UI_SETTINGS_MENU_ANTIALIASING_OPTION3, Anchor::Center, {188, 106}, ASSETS_FONT_PRIMARY_16_PX, "MSAA 4x", {92, 22}}, AntiAliasing::MSAA_4X},
		{{UI_SETTINGS_MENU_ANTIALIASING_OPTION4, Anchor::Center, {-188, 176}, ASSETS_FONT_PRIMARY_16_PX, "MSAA 8x", {92, 22}}, AntiAliasing::MSAA_8X}
	};

	// TRANSITIONS

	const float pi = 3.14159265f;

	static const Transition mainMenuSpaceshipPosTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {0.9f, 0.5f, 0.8f}, 23.0f, Easing::EaseInQuad},
		{{0.9f, 0.5f, 0.8f}, {1.2f, -0.3f, 0.5f}, 16.0f, Easing::EaseInOutQuad},
		{{1.2f, -0.3f, 0.5f}, {-0.4f, -0.6f, -0.9f}, 38.0f, Easing::EaseOutCubic},
		{{-0.4f, -0.6f, -0.9f}, {-0.6f, 0.2f, -0.5f}, 11.0f, Easing::EaseInCubic},
		{{-0.6f, 0.2f, -0.5f}, {0.1f, -0.05f, 0.15f}, 14.0f, Easing::EaseInOutSine},
		{{0.1f, -0.05f, 0.15f},  {0.0f, 0.0f, 0.0f}, 20.0f, Easing::EaseInOutSine}
	};

	static const Transition mainMenuSpaceshipRotTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {0.015f, 0.008f, 0.02f}, 14.0f, Easing::EaseInSine},
		{{0.015f, 0.008f, 0.02f}, {0.04f, 0.025f, 0.06f}, 8.0f,  Easing::EaseInCubic},
		{{0.04f, 0.025f, 0.06f}, {0.02f, -0.015f, 0.09f}, 12.0f, Easing::Linear},
		{{0.02f, -0.015f, 0.09f}, {-0.04f, -0.07f, 0.11f}, 9.0f, Easing::EaseInOutQuad},
		{{-0.04f, -0.07f, 0.11f}, {-0.06f, -0.09f, 0.08f}, 13.0f, Easing::EaseInOutSine},
		{{-0.06f, -0.09f, 0.08f}, {-0.03f, -0.05f, 0.04f}, 11.0f, Easing::EaseOutCubic},
		{{-0.03f, -0.05f, 0.04f}, {0.005f, -0.01f, 0.015f}, 9.0f, Easing::EaseOutSine},
		{{0.005f, -0.01f, 0.015f}, {0.008f, -0.003f, 0.012f}, 7.0f, Easing::EaseInOutSine},
		{{0.008f, -0.003f, 0.012f}, {0.0f, 0.0f, 0.0f}, 18.0f, Easing::EaseInOutSine}
	};

	static const Transition mainMenuAsteroid1PosTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {-3.5f, -3.0f, 8.5f}, 15.0f, Easing::EaseInOutQuad},
		{{-3.5f, -3.0f, 8.5f}, {-12.0f, -5.0f, 12.0f}, 15.0f, Easing::EaseInOutQuad},
		{{-12.0f, -5.0f, 12.0f}, {-20.5f, -3.0f, 8.5f}, 15.0f, Easing::EaseInOutQuad},
		{{-20.5f, -3.0f, 8.5f}, {-24.0f, 0.0f, 0.0f}, 15.0f, Easing::EaseInOutQuad},
		{{-24.0f, 0.0f, 0.0f}, {-20.5f, 2.0f, -8.5f}, 15.0f, Easing::EaseInOutQuad},
		{{-20.5f, 2.0f, -8.5f}, {-12.0f, 3.0f,-12.0f}, 15.0f, Easing::EaseInOutQuad},
		{{-12.0f, 3.0f, -12.0f}, {-3.5f, 2.0f, -8.5f}, 15.0f, Easing::EaseInOutQuad},
		{{-3.5f, 2.0f, -8.5f}, {0.0f, 0.0f, 0.0f}, 15.0f, Easing::EaseInOutQuad}
	};

	static const Transition mainMenuAsteroid1RotTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {0.8f, 1.2f, -0.5f}, 15.0f, Easing::Linear},
		{{0.8f, 1.2f, -0.5f}, {1.6f, 2.4f, -1.0f}, 15.0f, Easing::Linear},
		{{1.6f, 2.4f, -1.0f}, {2.4f, 3.6f, -1.5f}, 15.0f, Easing::Linear},
		{{2.4f, 3.6f, -1.5f}, {3.2f, 4.8f, -2.0f}, 15.0f, Easing::Linear},
		{{3.2f, 4.8f, -2.0f}, {4.0f, 6.0f, -2.5f}, 15.0f, Easing::Linear},
		{{4.0f, 6.0f, -2.5f}, {4.8f, 7.2f, -3.0f}, 15.0f, Easing::Linear},
		{{4.8f, 7.2f, -3.0f}, {5.6f, 8.4f, -3.5f}, 15.0f, Easing::Linear},
		{{5.6f, 8.4f, -3.5f}, {6.4f, 9.6f, -4.0f}, 15.0f, Easing::Linear}
	};

	static const Transition mainMenuAsteroid2PosTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {4.4f, 4.2f, 10.6f}, 18.0f, Easing::EaseInOutQuad},
		{{4.4f, 4.2f, 10.6f}, {15.0f, 6.0f, 15.0f}, 18.0f, Easing::EaseInOutQuad},
		{{15.0f, 6.0f, 15.0f}, {25.6f, 4.2f, 10.6f}, 18.0f, Easing::EaseInOutQuad},
		{{25.6f, 4.2f, 10.6f}, {30.0f, 0.0f, 0.0f}, 18.0f, Easing::EaseInOutQuad},
		{{30.0f, 0.0f,  0.0f}, {25.6f, -4.2f, -10.6f}, 18.0f, Easing::EaseInOutQuad},
		{{25.6f, -4.2f, -10.6f}, {15.0f, -6.0f, -15.0f}, 18.0f, Easing::EaseInOutQuad},
		{{15.0f, -6.0f, -15.0f}, {4.4f, -4.2f, -10.6f}, 18.0f, Easing::EaseInOutQuad},
		{{4.4f, -4.2f, -10.6f}, {0.0f, 0.0f, 0.0f}, 18.0f, Easing::EaseInOutQuad}
	};

	static const Transition mainMenuAsteroid2RotTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {-0.5f, 1.5f, 0.8f}, 12.0f, Easing::Linear},
		{{-0.5f, 1.5f, 0.8f}, {-1.0f, 3.0f, 1.6f}, 12.0f, Easing::Linear},
		{{-1.0f, 3.0f, 1.6f}, {-1.5f, 4.5f, 2.4f}, 12.0f, Easing::Linear},
		{{-1.5f, 4.5f, 2.4f}, {-2.0f, 6.0f, 3.2f}, 12.0f, Easing::Linear},
		{{-2.0f, 6.0f, 3.2f}, {-2.5f, 7.5f, 4.0f}, 12.0f, Easing::Linear},
		{{-2.5f, 7.5f, 4.0f}, {-3.0f, 9.0f, 4.8f}, 12.0f, Easing::Linear},
		{{-3.0f, 9.0f, 4.8f}, {-3.5f, 10.5f, 5.6f}, 12.0f, Easing::Linear},
		{{-3.5f, 10.5f, 5.6f}, {-4.0f, 12.0f, 6.4f}, 12.0f, Easing::Linear}
	};

	static const Transition mainMenuAsteroid3RotTrans[] = {
		{{0.0f, 0.0f, 0.0f}, {1.5f, 2.0f, -0.8f}, 40.0f, Easing::Linear},
		{{1.5f, 2.0f, -0.8f}, {3.0f, 4.0f, -1.6f}, 40.0f, Easing::Linear},
		{{3.0f, 4.0f, -1.6f}, {4.5f, 6.0f, -2.4f}, 40.0f, Easing::Linear},
		{{4.5f, 6.0f, -2.4f}, {6.0f, 8.0f, -3.2f}, 40.0f, Easing::Linear}
	};
}

#endif