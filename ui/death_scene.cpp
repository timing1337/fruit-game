#include "death_scene.h"

void DeathScene::Initialize() {
	Renderer* renderer = Renderer::getInstance();
	renderer->textures["ui/death_title"] = renderer->CreateText("YOU DIED", "Helvetica-Bold", 30, { 255, 255, 255, 255 });
	renderer->textures["ui/death_score_text"] = renderer->CreateText("You have sliced a total of ", "Helvetica-Bold", 20, { 255, 255, 255, 255 });
	renderer->textures["ui/button_try_again"] = renderer->CreateText("Again?", "Helvetica-Bold", 20, { 255, 255, 255, 255 });
	renderer->textures["ui/button_try_again_hover"] = renderer->CreateText("Again?", "Helvetica-Bold", 26, { 255, 255, 255, 255 });
	renderer->textures["ui/button_back_menu"] = renderer->CreateText("Return to menu", "Helvetica-Bold", 20, {255, 255, 255, 255});
	renderer->textures["ui/button_back_menu_hover"] = renderer->CreateText("Return to menu", "Helvetica-Bold", 26, { 255, 255, 255, 255 });
}

void DeathScene::Show() {
	Renderer* renderer = Renderer::getInstance();
	const SDL_Point center = SDL_Point{ renderer->width / 2, renderer->height / 2 };
	int y = center.y - 150;
	renderer->RenderTexture("ui/death_title", center.x, y);
	y += 30;
	renderer->RenderTexture("ui/death_score_text", center.x, y);
	renderer->RenderTexture(MainScene::scoreText, center.x + (renderer->textures["ui/death_score_text"]->width / 2), y, Alignment::RIGHT);
	y += 80;

	SDL_Point mousePos = SDL_Point{ 0, 0 };
	SDL_GetMouseState(&mousePos.x, &mousePos.y);

	GameTexture* tryAgainButton = renderer->GetTextureByName("ui/button_try_again");
	SDL_Point tryAgainButtonPos = SDL_Point{ center.x - tryAgainButton->width / 2, y};

	if (isPointInRect(mousePos, tryAgainButtonPos, tryAgainButton)) {
		renderer->RenderTexture("ui/button_try_again_hover", center.x, y);
	}
	else {
		renderer->RenderTexture("ui/button_try_again", center.x, y);
	}

	y += 10 + tryAgainButton->height;

	GameTexture* backMenuButton = renderer->GetTextureByName("ui/button_back_menu");
	SDL_Point backMenuButtonPos = SDL_Point{ center.x - backMenuButton->width / 2, y};
	if (isPointInRect(mousePos, backMenuButtonPos, backMenuButton)) {
		renderer->RenderTexture("ui/button_back_menu_hover", center.x, y);
	}
	else {
		renderer->RenderTexture("ui/button_back_menu", center.x, y);
	}
}

void DeathScene::OnMouseClick(SDL_MouseButtonEvent& e) {
	Renderer* renderer = Renderer::getInstance();
	const int width = renderer->width;
	const int height = renderer->height;
	const SDL_Point center{ width / 2, height / 2 };

	SDL_Point mousePos{ e.x, e.y };

	int y = center.y - 40;

	GameTexture* tryAgainButton = renderer->GetTextureByName("ui/button_try_again");
	SDL_Point tryAgainButtonPos = SDL_Point{ center.x - tryAgainButton->width / 2, y };

	if (GameManager::getInstance()->isOnStateChange) {
		return;
	}

	if (isPointInRect(mousePos, tryAgainButtonPos, tryAgainButton)) {
		GameManager::getInstance()->isOnStateChange = true;

		renderer->PlayFadeTransition(
			[](TimerTask* self) {
				GameManager::getInstance()->FireStateChange(GameState::STARTING);
			},
			[renderer](TimerTask* self) {
				renderer->PlayTitleAnimationAndStartGame();
				GameManager::getInstance()->isOnStateChange = false;
			}
		);
	}

	y += 10 + tryAgainButton->height;

	GameTexture* backMenuButton = renderer->GetTextureByName("ui/button_back_menu");
	SDL_Point backMenuButtonPos = SDL_Point{ center.x - backMenuButton->width / 2, y};
	if (isPointInRect(mousePos, backMenuButtonPos, backMenuButton)) {
		GameManager::getInstance()->isOnStateChange = true;
		renderer->PlayFadeTransition(
			[](TimerTask* self) {
				GameManager::getInstance()->FireStateChange(GameState::WAITING);
			},
			[](TimerTask* self) {
				GameManager::getInstance()->isOnStateChange = false;
			}
		);
	}
}