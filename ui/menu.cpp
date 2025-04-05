#include "menu.h"

void MainMenu::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/button_play"] = renderer->CreateText("Play", "Helvetica-Bold", 24, { 255, 255, 255, 255 });
	renderer->textures["ui/button_play_hover"] = renderer->CreateText("Play", "Helvetica-Bold", 40, { 255, 255, 255, 255 });
}

void MainMenu::Show() {
	Renderer* renderer = Renderer::getInstance();
	const SDL_Point center = SDL_Point{ renderer->width / 2, renderer->height / 2 };
	SDL_Point mousePos = SDL_Point{ 0, 0 };
	
	SDL_GetMouseState(&mousePos.x, &mousePos.y);

	GameTexture* playButton = renderer->GetTextureByName("ui/button_play");

	SDL_Point playButtonPos = SDL_Point{ center.x - playButton->text->width / 2, center.y };

	if (isPointInRect(mousePos, playButtonPos, playButton)) {
		renderer->RenderText("ui/button_play_hover", center.x, center.y);
	}
	else {
		renderer->RenderText("ui/button_play", center.x, center.y);
	}
}

void MainMenu::OnMouseClick(SDL_MouseButtonEvent& e) {
	Renderer* renderer = Renderer::getInstance();
	const int width = renderer->width;
	const int height = renderer->height;
	const SDL_Point center{ width / 2, height / 2 };

	SDL_Point mousePos{ e.x, e.y };

	GameTexture* playButton = renderer->GetTextureByName("ui/button_play");

	SDL_Point playButtonPos = SDL_Point{ center.x - playButton->text->width / 2, center.y };

	if (isPointInRect(mousePos, playButtonPos, playButton)) {
		renderer->PlayFadeTransition(
		[](TimerTask* self) {
			SDL_Log("Starting game");
			GameManager::getInstance()->FireStateChange(GameState::STARTING);
		},
		[renderer](TimerTask* self) {
			renderer->PlayTitleAnimationAndStartGame();
		});
	}
}