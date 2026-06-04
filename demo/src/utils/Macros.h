#ifndef MACROS_H
#define MACROS_H
#include <math/Vec3.h>

#define TARGET_RESOLUTION_WIDTH 1920
#define TARGET_RESOLUTION_HEIGHT 1080

#define SUN_COLOR Vec3{1.0f, 0.95f, 0.85f}
#define SUN_DIRECTION Vec3{0.065f, 3.07f, 0.0f}

#define ASSETS_FONT_PRIMARY_24_PX "demo/assets/fonts/arial_24.spritefont"
#define ASSETS_FONT_PRIMARY_16_PX "demo/assets/fonts/arial_16.spritefont"
#define ASSETS_FONT_PRIMARY_12_PX "demo/assets/fonts/arial_12.spritefont"
#define ASSETS_SKYBOX "demo/assets/skybox/starfield.json"
#define ASSETS_MAIN_MENU_PANEL "demo/assets/textures/ui/main_menu.png"
#define ASSETS_SETTINGS_MENU_PANEL "demo/assets/textures/ui/settings_menu.png"
#define ASSETS_SPACESHIP_MESH "demo/assets/meshes/spaceship.obj"
#define ASSETS_SPACESHIP_MATERIAL "demo/assets/materials/spaceship.json"

#define ENTITIES_MAIN_CAMERA "main_camera"
#define ENTITIES_GLOBAL_SCRIPTS "global_scripts"
#define ENTITIES_SUN "sun"

#define UI_HINT1 "hint1"
#define UI_HINT2 "hint2"
#define UI_HINT3 "hint3"
#define UI_HINT4 "hint4"

#define UI_INFO_FPS "fps"

#define UI_MAIN_MENU_PANEL "main_menu_panel"
#define UI_MAIN_MENU_TITLE "main_menu_title"
#define UI_MAIN_MENU_START_BUTTON "main_menu_start_button"
#define UI_MAIN_MENU_QUIT_BUTTON "main_menu_quit_button"
#define UI_MAIN_MENU_SETTINGS_BUTTON "main_menu_settings_button"

#define UI_SETTINGS_MENU_PANEL "settings_menu_panel"
#define UI_SETTINGS_MENU_TITLE "settings_menu_title"
#define UI_SETTINGS_MENU_BACK_BUTTON "settings_menu_back_button"
#define UI_SETTINGS_MENU_TEX_FILTERING_LABEL "settings_menu_texture_filtering_label"
#define UI_SETTINGS_MENU_TEX_FILTERING_OPTION1 "settings_menu_texture_filtering_option1"
#define UI_SETTINGS_MENU_TEX_FILTERING_OPTION2 "settings_menu_texture_filtering_option2"
#define UI_SETTINGS_MENU_TEX_FILTERING_OPTION3 "settings_menu_texture_filtering_option3"
#define UI_SETTINGS_MENU_TEX_FILTERING_OPTION4 "settings_menu_texture_filtering_option4"
#define UI_SETTINGS_MENU_ANTIALIASING_LABEL "settings_menu_antialiasing_label"
#define UI_SETTINGS_MENU_ANTIALIASING_OPTION1 "settings_menu_antialiasing_option1"
#define UI_SETTINGS_MENU_ANTIALIASING_OPTION2 "settings_menu_antialiasing_option2"
#define UI_SETTINGS_MENU_ANTIALIASING_OPTION3 "settings_menu_antialiasing_option3"
#define UI_SETTINGS_MENU_ANTIALIASING_OPTION4 "settings_menu_antialiasing_option4"

#endif