#include "game.h"

GameManager* GameManager::instancePtr = new GameManager();

void GameManager::Initialize() {
	this->mt.seed(time(0));
	this->comboDistribution = uniform_real_distribution<float>(0, 1);
}

void GameManager::Heartbeat(int deltaTicks) {
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
					
					Enemy* enemy = new Enemy(vec2_t(x, y), 0, 0);
					enemy->SetHitbox({ 20, 20 });
					EntityManager::getInstance()->spawnEntity(enemy);
				break;
			}
			break;
		}
	}

	for (int i = 0; i < this->mousePathRecordsLeftover.size(); i++) {
		MousePathRecord* record = this->mousePathRecordsLeftover[i];
		if (record->paths.size() == 0) {
			delete record;
			this->mousePathRecordsLeftover.erase(this->mousePathRecordsLeftover.begin() + i);
			continue;
		}

		for (int j = 0; j < record->paths.size(); j++) {
			MousePath* path = &record->paths[j];
			path->longevity -= this->deltaTime;
			if (path->longevity <= 0) {
				record->paths.erase(record->paths.begin() + j);
			}
		}

		record->RecalculateDistance();
	}

	if (this->mousePathRecord != nullptr) {
		for (int i = 0; i < this->mousePathRecord->paths.size(); i++) {
			MousePath* path = &this->mousePathRecord->paths[i];
			path->longevity -= this->deltaTime;
			if (path->longevity <= 0) {
				this->mousePathRecord->paths.erase(this->mousePathRecord->paths.begin() + i);
			}
		}
		this->mousePathRecord->RecalculateDistance();
	}

	if (comboExpirationTick <= 0) {
		if (currentCombo != 0) SetCombo(0);
	}
	else {
		comboExpirationTick -= deltaTicks;
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

	this->mousePathRecord = new MousePathRecord();
	this->mousePathRecord->AddPoint(SDL_Point{ e.x, e.y });
}

void GameManager::OnMouseRelease(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}

	if (this->state != GameState::RUNNING) {
		return;
	}

	if (this->mousePathRecord == nullptr) {
		return;
	}

	if (e.x < 0 || e.y < 0 || e.x > Renderer::getInstance()->width || e.y > Renderer::getInstance()->height) {
		return;
	}

	this->mousePathRecord->AddPoint(SDL_Point{ e.x, e.y });
	this->mousePathRecordsLeftover.push_back(this->mousePathRecord);
	this->mousePathRecord = nullptr;
}

void GameManager::OnMouseMove(SDL_MouseButtonEvent& e) {
	if (this->state != GameState::RUNNING) {
		return;
	}

	if (this->mousePathRecord == nullptr) {
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

			for (int i = 0; i < entity_mgr->entities.size(); i++) {
				Entity* entity = entity_mgr->entities[i];
				if (!entity->alive) continue;
				if (entity->IsColliding(point.x, point.y)) {
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
	SDL_Log("GAME STATUS: STARTING");
	SDL_Log("RESETTING GAME DATA");
	this->remainingLives = 3;
	SetScore(0);
}

void GameManager::OnStarting() {
	SDL_Log("GAME STATUS: STARTING");
	SDL_Log("RESETTING GAME DATA");
	this->remainingLives = 3;
	SetScore(0);
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

void GameManager::SetScore(int score) {
	SDL_Log("Updating score: %d", score);
	this->score = score;
	//update spawn interval
	EntityManager::getInstance()->spawnTask->interval = max((int)(1000 - this->score * 0.01), 600);
	MainScene::UpdateScoreText();
}

void GameManager::AddScore(int score) {

	float multiplicationChance = max(0.01f + currentCombo * 0.05f, 0.5f);
	if (this->comboDistribution(this->mt) < multiplicationChance) {
		score *= 2;
	}
	SetScore(this->score + score );
}

void GameManager::SetCombo(int combo) {
	currentCombo = combo;

	if (currentCombo == 0) {
		comboExpirationTick = 0;
	}
	else {
		comboExpirationTick = max(1000 + currentCombo * 20, 2000);
		if (currentCombo > maxComboReached) {
			maxComboReached = currentCombo;
		}
	}
	MainScene::UpdateComboText();
}

void GameManager::AddCombo(int combo) {
	SetCombo(currentCombo + combo);
}