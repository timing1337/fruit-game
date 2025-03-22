#include "game.h"

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
			this->OnMouseClick(e.button);
			break;
		case SDL_MOUSEBUTTONUP:
			this->OnMouseRelease(e.button);
			break;
		case SDL_MOUSEMOTION:
			this->OnMouseMove(e.button);
			break;
		case SDL_KEYDOWN:

			switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					this->running = false;
				break;
				case SDLK_INSERT:
					//Spawn entity
					if (this->state != GameState::RUNNING) {
						break;
					}
					int x, y;
					SDL_GetMouseState(&x, &y);
				break;
			}
			break;
		}
	}

	for (int i = 0; i < this->mousePathRecord->paths.size(); i++) {
		MousePath* path = &this->mousePathRecord->paths[i];
		path->longevity -= this->deltaTime;
		if (path->longevity < 0) {
			this->mousePathRecord->paths.erase(this->mousePathRecord->paths.begin() + i);
		}
	}
}

void GameManager::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}

	if (this->state != GameState::RUNNING) {
		return;
	}

	if (e.x < 0 || e.y < 0 || e.x > Renderer::getInstance()->width || e.y > Renderer::getInstance()->height) {
		return;
	}

	this->mousePathRecord->isRecording = true;
	this->mousePathRecord->AddPoint(SDL_Point{ e.x, e.y });
}

void GameManager::OnMouseRelease(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}

	if (this->state != GameState::RUNNING) {
		return;
	}

	if (!this->mousePathRecord->isRecording) {
		return;
	}

	this->mousePathRecord->isRecording = false;

	if (e.x < 0 || e.y < 0 || e.x > Renderer::getInstance()->width || e.y > Renderer::getInstance()->height) {
		return;
	}
	this->mousePathRecord->AddPoint(SDL_Point{ e.x, e.y });
}

void GameManager::OnMouseMove(SDL_MouseButtonEvent& e) {
	if (this->state != GameState::RUNNING) {
		return;
	}

	if (!this->mousePathRecord->isRecording) {
		return;
	}

	if (e.x < 0 || e.y < 0 || e.x > Renderer::getInstance()->width || e.y > Renderer::getInstance()->height) {
		return;
	}
	this->mousePathRecord->AddPoint({ e.x, e.y });
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
}

void GameManager::OnRunning() {
}

void GameManager::OnPostgame() {
}

void GameManager::UpdateScore(int score) {
	this->score += score;
	MainScene::UpdateScoreText();
}