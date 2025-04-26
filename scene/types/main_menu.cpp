#include "main_menu.h"

MainMenu::MainMenu() : BaseScene(SceneId::MAIN_MENU) {
	this->AddText("main_title", vec2_t{ RENDERER_CENTER_X, 100 }, "FRUIT NINJA", "Helvetica-Bold", 40, { 255, 255, 255, 255 });

	this->AddButton("start_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y - 60 }, "START", "NoyhR-Light", 25, { 255, 255, 255, 255 }, []() {
	}, 0, {0, 0, 0, 255}, 10);
	this->AddButton("settings_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y }, "SETTINGS", "NoyhR-Light", 24, { 255, 255, 255, 255 }, []() {
		
	}, 0, { 0, 0, 0, 255 }, 10);
	this->AddButton("quit_button", vec2_t{ RENDERER_CENTER_X, RENDERER_CENTER_Y + 60 }, "QUIT", "NoyhR-Light", 24, { 255, 255, 255, 255 }, []() {}, 0, { 0, 0, 0, 255 }, 10);
}