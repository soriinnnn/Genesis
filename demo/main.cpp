#include "src/Demo.h"
#include "src/scripts/MainMenu.h"
#include "src/utils/GameState.h"	
#include "src/utils/Constants.h"
#include "src/utils/Macros.h"
#include "src/utils/Types.h"
#include "src/utils/Utils.h"

#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <entity/components/CameraComponent.h>	
#include <entity/components/LightComponent.h>
#include <entity/components/RigidBodyComponent.h>
#include <entity/components/ScriptComponent.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIPanel.h>
#include <ui/elements/UIButton.h>
#include <ui/elements/UIImage.h>
#include <physics/RigidBody.h>
#include <resources/Mesh.h>
#include <resources/Material.h>
#include <resources/Font.h>

using namespace genesis;
using namespace constants;
using namespace types;
using namespace utils;

static void createMainMenu(Game& game)
{
	GameContext context = game.getContext();

	UIImageCreateInfo menuPanelInfo = {
		UI_MAIN_MENU_PANEL,
		ASSETS_MAIN_MENU_PANEL,
		{1.0f, 1.0f, 1.0f, 0.5f},
		Anchor::Center,
		{0, 0}
	};
	createUIImage(game, menuPanelInfo);

	UILabelCreateInfo menuTitleInfo = {
		UI_MAIN_MENU_TITLE,
		ASSETS_FONT_PRIMARY_24_PX,
		"STARFIELD",
		{180, 30},
		Anchor::Center,
		{0, -150}
	};
	createUILabel(game, menuTitleInfo);

	/* START BUTTON */

	UIButtonCreateInfo menuStartInfo = {
		UI_MAIN_MENU_START_BUTTON,
		Anchor::Center,
		{0, -38},
		ASSETS_FONT_PRIMARY_16_PX,
		"START",
		{72, 22}
	};
	UIButton* menuStart = createUIButton(game, menuStartInfo);

	menuStart->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_MAIN_MENU_START_BUTTON)->setColor(buttonHoverColor);
	});
	menuStart->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_MAIN_MENU_START_BUTTON)->setColor(buttonIdleColor);
	});

	/* SETTINGS BUTTON */

	UIButtonCreateInfo menuSettingsInfo = {
		UI_MAIN_MENU_SETTINGS_BUTTON,
		Anchor::Center,
		{0, 50},
		ASSETS_FONT_PRIMARY_16_PX,
		"SETTINGS",
		{110, 22}
	};
	UIButton* menuSettings = createUIButton(game, menuSettingsInfo);

	menuSettings->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_MAIN_MENU_SETTINGS_BUTTON)->setColor(buttonHoverColor);
	});
	menuSettings->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_MAIN_MENU_SETTINGS_BUTTON)->setColor(buttonIdleColor);
	});
	menuSettings->setOnMouseUpCallback([context](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		for (const String& element : mainMenuElements) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(false);
		}
		for (const String& element : settingsMenuElements) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(true);
		}
	});

	/* QUIT BUTTON */

	UIButtonCreateInfo menuQuitInfo = {
		UI_MAIN_MENU_QUIT_BUTTON,
		Anchor::Center,
		{0, 137},
		ASSETS_FONT_PRIMARY_16_PX,
		"QUIT",
		{54, 22}
	};
	UIButton* menuQuit = createUIButton(game, menuQuitInfo);

	menuQuit->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_MAIN_MENU_QUIT_BUTTON)->setColor(buttonHoverColor);
	});
	menuQuit->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_MAIN_MENU_QUIT_BUTTON)->setColor(buttonIdleColor);
	});
	menuQuit->setOnMouseUpCallback([&](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		game.quit();
	});
}

static void updateTextureFilteringOptions(Game& game)
{
	GameContext context = game.getContext();
	TextureFiltering currentFilter = game.getTextureFiltering();

	for (const auto& option : textureFilterOptions) {
		UIElement* element = context.ui.getElement<UIElement>(option.buttonInfo.id);

		bool isSelected = (currentFilter == option.textureFilter);
		bool isHovered = element->isHovered();

		if (isSelected) {
			element->setColor(isHovered ? buttonSelectedHoverColor : buttonSelectedIdleColor);
		}
		else {
			element->setColor(isHovered ? buttonHoverColor : buttonIdleColor);
		}
	}
}

static void createSettingsTextureFilteringOptions(Game& game)
{
	GameContext context = game.getContext();

	UILabelCreateInfo labelInfo = {
		UI_SETTINGS_MENU_TEX_FILTERING_LABEL,
		ASSETS_FONT_PRIMARY_16_PX,
		"Texture filtering:",
		{154, 22},
		Anchor::Center,
		{-186, -152}
	};
	createUILabel(game, labelInfo);

	for (const auto& option : textureFilterOptions) {
		UIButton* button = createUIButton(game, option.buttonInfo);

		button->setOnMouseEnterCallback([&]() {
			GameContext context = game.getContext();
			if (game.getTextureFiltering() != option.textureFilter) {
				context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonHoverColor);
				return;
			}
			context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonSelectedHoverColor);
		});
		button->setOnMouseOutCallback([&]() {
			GameContext context = game.getContext();
			if (game.getTextureFiltering() != option.textureFilter) {
				context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonIdleColor);
				return;
			}
			context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonSelectedIdleColor);
		});
		button->setOnMouseUpCallback([&](MouseButton mouseButton) {
			if (mouseButton != MouseButton::Left) {
				return;
			}
			game.setTextureFiltering(option.textureFilter);
			updateTextureFilteringOptions(game);
		});
	}

	updateTextureFilteringOptions(game);
}

static void updateAntiAliasingOptions(Game& game)
{
	GameContext context = game.getContext();
	AntiAliasing currentAntialiasing = game.getAntiAliasing();

	for (const auto& option : antiAliasingOptions) {
		UIElement* element = context.ui.getElement<UIElement>(option.buttonInfo.id);

		bool isSelected = (currentAntialiasing == option.antiAliasing);
		bool isHovered = element->isHovered();

		if (isSelected) {
			element->setColor(isHovered ? buttonSelectedHoverColor : buttonSelectedIdleColor);
		}
		else {
			element->setColor(isHovered ? buttonHoverColor : buttonIdleColor);
		}
	}
}

static void createSettingsAntiAliasingOptions(Game& game)
{
	GameContext context = game.getContext();

	UILabelCreateInfo labelInfo = {
		UI_SETTINGS_MENU_ANTIALIASING_LABEL,
		ASSETS_FONT_PRIMARY_16_PX,
		"Anti-aliasing:",
		{124, 22},
		Anchor::Center,
		{-201, 50}
	};
	createUILabel(game, labelInfo);

	for (const auto& option : antiAliasingOptions) {
		UIButton* button = createUIButton(game, option.buttonInfo);

		button->setOnMouseEnterCallback([&]() {
			GameContext context = game.getContext();
			if (game.getAntiAliasing() != option.antiAliasing) {
				context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonHoverColor);
				return;
			}
			context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonSelectedHoverColor);
		});
		button->setOnMouseOutCallback([&]() {
			GameContext context = game.getContext();
			if (game.getAntiAliasing() != option.antiAliasing) {
				context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonIdleColor);
				return;
			}
			context.ui.getElement<UIElement>(option.buttonInfo.id)->setColor(buttonSelectedIdleColor);
		});
		button->setOnMouseUpCallback([&](MouseButton mouseButton) {
			if (mouseButton != MouseButton::Left) {
				return;
			}
			game.setAntiAliasing(option.antiAliasing);
			updateAntiAliasingOptions(game);
		});
	}

	updateAntiAliasingOptions(game);
}

static void createSettingsMenu(Game& game)
{
	GameContext context = game.getContext();

	UIImageCreateInfo menuPanelInfo = {
		UI_SETTINGS_MENU_PANEL,
		ASSETS_SETTINGS_MENU_PANEL,
		{1.0f, 1.0f, 1.0f, 0.5f},
		Anchor::Center,
		{0, 0}
	};
	createUIImage(game, menuPanelInfo);

	UILabelCreateInfo menuTitleInfo = {
		UI_SETTINGS_MENU_TITLE,
		ASSETS_FONT_PRIMARY_24_PX,
		"SETTINGS",
		{164, 30},
		Anchor::Center,
		{0, -250}
	};
	createUILabel(game, menuTitleInfo);

	UIButtonCreateInfo menuBackInfo = {
		UI_SETTINGS_MENU_BACK_BUTTON,
		Anchor::Center,
		{188, 238},
		ASSETS_FONT_PRIMARY_16_PX,
		"BACK",
		{60, 22}
	};
	UIButton* menuBack = createUIButton(game, menuBackInfo);

	menuBack->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_SETTINGS_MENU_BACK_BUTTON)->setColor(buttonHoverColor);
	});
	menuBack->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_SETTINGS_MENU_BACK_BUTTON)->setColor(buttonIdleColor);
	});
	menuBack->setOnMouseUpCallback([context](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		if (gameState == GameState::MainMenu) {
			for (const String& element : mainMenuElements) {
				context.ui.getElement<UIElement>(element.c_str())->setVisible(true);
			}
		}
		else if (gameState == GameState::Paused) {
			for (const String& element : gameMenuElements) {
				context.ui.getElement<UIElement>(element.c_str())->setVisible(true);
			}
		}
		for (const String& element : settingsMenuElements) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(false);
		}
	});

	createSettingsTextureFilteringOptions(game);
	createSettingsAntiAliasingOptions(game);
	for (const String& element : settingsMenuElements) {
		context.ui.setZOrder(element.c_str(), 1);
	}
}

static void createGameMenu(Game& game)
{
	GameContext context = game.getContext();

	UIImageCreateInfo menuPanelInfo = {
		UI_GAME_MENU_PANEL,
		ASSETS_GAME_MENU_PANEL,
		{1.0f, 1.0f, 1.0f, 0.5f},
		Anchor::Center,
		{0, 0}
	};
	createUIImage(game, menuPanelInfo);

	UILabelCreateInfo menuTitleInfo = {
		UI_GAME_MENU_TITLE,
		ASSETS_FONT_PRIMARY_24_PX,
		"PAUSE",
		{116, 30},
		Anchor::Center,
		{0, -150}
	};
	createUILabel(game, menuTitleInfo);

	/* RESUME BUTTON */

	UIButtonCreateInfo menuResumeInfo = {
		UI_GAME_MENU_RESUME_BUTTON,
		Anchor::Center,
		{0, -38},
		ASSETS_FONT_PRIMARY_16_PX,
		"RESUME",
		{96, 22}
	};
	UIButton* menuResume = createUIButton(game, menuResumeInfo);

	menuResume->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_GAME_MENU_RESUME_BUTTON)->setColor(buttonHoverColor);
	});
	menuResume->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_GAME_MENU_RESUME_BUTTON)->setColor(buttonIdleColor);
	});
	menuResume->setOnMouseUpCallback([context](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		if (gameState != GameState::Paused) {
			return;
		}
		gameState = GameState::Playing;
		for (const String& element : gameMenuElements) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(false);
		}
		for (const String& element : mainMenuHints) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(false);
		}
	});

	/* SETTINGS BUTTON */

	UIButtonCreateInfo menuSettingsInfo = {
		UI_GAME_MENU_SETTINGS_BUTTON,
		Anchor::Center,
		{0, 50},
		ASSETS_FONT_PRIMARY_16_PX,
		"SETTINGS",
		{110, 22}
	};
	UIButton* menuSettings = createUIButton(game, menuSettingsInfo);

	menuSettings->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_GAME_MENU_SETTINGS_BUTTON)->setColor(buttonHoverColor);
	});
	menuSettings->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_GAME_MENU_SETTINGS_BUTTON)->setColor(buttonIdleColor);
	});
	menuSettings->setOnMouseUpCallback([context](MouseButton button) {
		if (button != MouseButton::Left) {
			return;
		}
		for (const String& element : gameMenuElements) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(false);
		}
		for (const String& element : settingsMenuElements) {
			context.ui.getElement<UIElement>(element.c_str())->setVisible(true);
		}
	});

	/* QUIT BUTTON */

	UIButtonCreateInfo menuMainInfo = {
		UI_GAME_MENU_MAIN_BUTTON,
		Anchor::Center,
		{0, 137},
		ASSETS_FONT_PRIMARY_16_PX,
		"MAIN MENU",
		{120, 22}
	};
	UIButton* menuMain = createUIButton(game, menuMainInfo);

	menuMain->setOnMouseEnterCallback([context]() {
		context.ui.getElement<UIElement>(UI_GAME_MENU_MAIN_BUTTON)->setColor(buttonHoverColor);
	});
	menuMain->setOnMouseOutCallback([context]() {
		context.ui.getElement<UIElement>(UI_GAME_MENU_MAIN_BUTTON)->setColor(buttonIdleColor);
	});

	for (const String& element : gameMenuElements) {
		context.ui.setZOrder(element.c_str(), 1);
	}
}

static void createGameInfo(Game& game)
{
	GameContext context = game.getContext();

	UILabelCreateInfo fpsInfo = {
		UI_INFO_FPS,
		ASSETS_FONT_PRIMARY_16_PX,
		"",
		{200, 22},
		Anchor::TopLeft,
		{0, 0}
	};
	createUILabel(game, fpsInfo);
}

static void createHints(Game& game)
{
	GameContext context = game.getContext();

	UILabelCreateInfo hint1Info = {
		UI_HINT1,
		ASSETS_FONT_PRIMARY_12_PX,
		"Press F11 to toggle fullscreen",
		{216, 16},
		Anchor::BottomLeft,
		{0, -4}
	};
	createUILabel(game, hint1Info);

	UILabelCreateInfo hint2Info = {
		UI_HINT2,
		ASSETS_FONT_PRIMARY_12_PX,
		"Press F2 to toggle vsync",
		{180, 16},
		Anchor::BottomLeft,
		{0, -24}
	};
	createUILabel(game, hint2Info);

	UILabelCreateInfo hint3Info = {
		UI_HINT3,
		ASSETS_FONT_PRIMARY_12_PX,
		"Press F1 to show fps",
		{164, 16},
		Anchor::BottomLeft,
		{0, -44}
	};
	createUILabel(game, hint3Info);

	UILabelCreateInfo hint4Info = {
		UI_HINT4,
		ASSETS_FONT_PRIMARY_12_PX,
		"Press G to toggle mouse lock",
		{210, 16},
		Anchor::BottomLeft,
		{0, -64}
	};
	createUILabel(game, hint4Info);

	for (const String& hint : mainMenuHints) {
		context.ui.setZOrder(hint.c_str(), 5);
	}
}

static void createLights(Game& game)
{
	GameContext context = game.getContext();

	Entity* sun = context.entities.createEntity(ENTITIES_SUN);

	LightComponent* sunLight = sun->createComponent<LightComponent>();
	sunLight->setType(LightComponent::LightType::Directional);
	sunLight->setColor(SUN_COLOR);
	sunLight->setIntensity(SUN_INTENSITY);

	TransformComponent* sunTransform = sun->getComponent<TransformComponent>();
	sunTransform->setRotation(SUN_DIRECTION);
}

static void createCamera(Game& game)
{
	GameContext context = game.getContext();

	Entity* camera = context.entities.createEntity(ENTITIES_MAIN_CAMERA);
	camera->createComponent<CameraComponent>();
}

static void setupGame(Game& game) 
{
	createCamera(game);
	createLights(game);
	createMainMenu(game);
	createSettingsMenu(game);
	createGameMenu(game);
	createGameInfo(game);
	createHints(game);
}

int main()
{
	try {
		Demo game({.windowTitle = "Starfield", .windowIcon = "C:/Users/Sorin/Downloads/draven_bof.ico", .logLevel = Logger::LogLevel::Info});
		setupGame(game);
		game.run();
	}
	catch (const std::runtime_error&) {
		return EXIT_FAILURE;
	}
	catch (const std::invalid_argument&) {
		return EXIT_FAILURE;
	}
	catch (const std::exception&) {
		return EXIT_FAILURE;
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}