#ifndef MACROS_H
#define MACROS_H
#include <math/Vec3.h>

// MISC

#define WINDOW_TITLE "Starfield"
#define WINDOW_ICON "C:/Users/Sorin/Downloads/draven_bof.ico"

#define SUN_COLOR Vec3{1.0f, 0.95f, 0.85f}
#define SUN_DIRECTION Vec3{0.065f, 3.07f, 0.0f}
#define SUN_INTENSITY 1.0f

// ASSETS

#define ASSETS_FONT_PRIMARY_24_PX "demo/assets/fonts/arial_24.spritefont"
#define ASSETS_FONT_PRIMARY_16_PX "demo/assets/fonts/arial_16.spritefont"
#define ASSETS_FONT_PRIMARY_12_PX "demo/assets/fonts/arial_12.spritefont"
#define ASSETS_SKYBOX "demo/assets/skybox/starfield.json"

#define ASSETS_MESH_SPACESHIP "demo/assets/meshes/spaceship.obj"
#define ASSETS_MESH_ASTEROID_1A "demo/assets/meshes/asteroid1a.obj"
#define ASSETS_MESH_ASTEROID_1B "demo/assets/meshes/asteroid1b.obj"
#define ASSETS_MESH_ASTEROID_2A "demo/assets/meshes/asteroid2a.obj"
#define ASSETS_MESH_ASTEROID_2B "demo/assets/meshes/asteroid2b.obj"
#define ASSETS_MESH_ASTEROID_2C "demo/assets/meshes/asteroid2c.obj"

#define ASSETS_MATERIAL_SPACESHIP "demo/assets/materials/spaceship.json"
#define ASSETS_MATERIAL_ASTEROID_1A "demo/assets/materials/asteroid1a.json"
#define ASSETS_MATERIAL_ASTEROID_1B "demo/assets/materials/asteroid1b.json"
#define ASSETS_MATERIAL_ASTEROID_2A "demo/assets/materials/asteroid2a.json"
#define ASSETS_MATERIAL_ASTEROID_2B "demo/assets/materials/asteroid2b.json"
#define ASSETS_MATERIAL_ASTEROID_2C "demo/assets/materials/asteroid2c.json"

#define ASSETS_MAIN_MENU_PANEL "demo/assets/textures/ui/main_menu.png"
#define ASSETS_SETTINGS_MENU_PANEL "demo/assets/textures/ui/settings_menu.png"
#define ASSETS_GAME_MENU_PANEL "demo/assets/textures/ui/game_menu.png"

// ENTITIES

#define ENTITIES_MAIN_CAMERA "main_camera"
#define ENTITIES_GLOBAL_SCRIPTS "global_scripts"
#define ENTITIES_SUN "sun"

// USER INTERFACE

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

#define UI_GAME_MENU_PANEL "game_menu_panel_button"
#define UI_GAME_MENU_TITLE "game_menu_title"
#define UI_GAME_MENU_RESUME_BUTTON "game_menu_resume_button"
#define UI_GAME_MENU_SETTINGS_BUTTON "game_menu_settings_button"
#define UI_GAME_MENU_MAIN_BUTTON "game_menu_main_button"

#endif