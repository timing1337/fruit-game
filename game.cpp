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
					
					EntityManager::getInstance()->spawnEntity<Enemy>(vec2_t(x, y), 0, 0);
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

	EntityManager* entity_mgr = EntityManager::getInstance();

	SDL_Point currentPoint{ e.x, e.y };

	if (this->mousePathRecord->paths.size() >= 2) {
		vector<MousePath> paths = this->mousePathRecord->paths;
		vector<SDL_Point> linePoints = getLinePoints(paths[paths.size() - 1].point, currentPoint);

		for (auto& point : linePoints) {
			for (auto& entity : EntityManager::getInstance()->entities) {
				if (!entity->alive) continue;
				if (entity->IsCollidingWithPoint(point.x, point.y)) {
					entity->onHit();
				}
			}
		}
	}

	this->mousePathRecord->AddPoint(currentPoint);
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
	SDL_Log("GAME STATUS: STARTING");
	SDL_Log("RESETTING GAME DATA");
	this->remainingLives = 3;
	this->score = 0;
}

void GameManager::OnRunning() {
	SDL_Log("GAME STATUS: RUNNING");
}

void GameManager::OnPostgame() {
	SDL_Log("GAME STATUS: POSTGAME");
	EntityManager* entity_mgr = EntityManager::getInstance();

	for (int i = 0; i < entity_mgr->entities.size(); i++) {
		Entity* entity = entity_mgr->entities[i];
		if (entity->alive) {
			entity->despawn(EntityDeathType::UNKNOWN);
		}
	}
}

void GameManager::UpdateScore(int score) {
	SDL_Log("Updating score: %d", score);
	this->score += score;
	//update spawn interval
	EntityManager::getInstance()->spawnTask->interval = max((int)(500 - this->score * 0.06), 300);
	MainScene::UpdateScoreText();
}