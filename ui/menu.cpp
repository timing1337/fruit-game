#include "menu.h"

void MainMenu::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/button_play"] = renderer->CreateText("Play", "Helvetica-Bold", 24, { 255, 255, 255, 255 });
	renderer->textures["ui/button_play_hover"] = renderer->CreateText("Play", "Helvetica-Bold", 40, { 255, 255, 255, 255 });
}

void MainMenu::Show() {
	Renderer* renderer = Renderer::getInstance();
	const vec2_t center = vec2_t(renderer->width / 2, renderer->height / 2);
	vec2_t mousePos = vec2_t(0, 0);
	
	SDL_GetMouseState(&mousePos.x, &mousePos.y);

	GameTexture* playButton = renderer->GetTextureByName("ui/button_play");

	if (mousePos.x >= center.x - playButton->text->width / 2 && mousePos.x <= center.x + playButton->text->width / 2 &&
		mousePos.y >= center.y - playButton->text->height / 2 && mousePos.y <= center.y + playButton->text->height / 2) {
		renderer->RenderText("ui/button_play_hover", center.x, center.y);
	}
	else {
		renderer->RenderText("ui/button_play", center.x, center.y);
	}
}