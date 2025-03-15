#include "game.h"

#include "animation/animation.h"

GameManager* GameManager::instancePtr = new GameManager();

void GameManager::Heartbeat() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) {
		case SDL_QUIT:
			this->running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			Renderer::getInstance()->OnMouseClick(e.button);
			break;
		}
	}
}

void GameManager::FireStateChange(GameState state) {
	this->state = state;
	switch (state) {
	case GameState::WAITING:
		OnWaiting();
		break;
	case GameState::STARTING:
		OnStarting();
		break;
	case GameState::RUNNING:
		OnRunning();
		break;
	case GameState::POSTGAME:
		OnPostgame();
		break;
	}
}

void GameManager::OnWaiting() {
}

void GameManager::OnStarting() {
	this->remainingLives = 3;
	this->score = 0;
	this->ticks = 0;
}

void GameManager::OnRunning() {
	this->ticks++;
}

void GameManager::OnPostgame() {
}