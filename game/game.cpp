#include "game/game.h"

GameManager* GameManager::instancePtr = new GameManager();

void GameManager::Initialize() {
	this->gameData = new GameData("game_data.dat");
}

void GameManager::Heartbeat(int deltaTicks) {
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

	if (e.x < 0 || e.y < 0 || e.x > RENDERER_WIDTH || e.y > RENDERER_HEIGHT) {
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

	if (e.x < 0 || e.y < 0 || e.x > RENDERER_WIDTH || e.y > RENDERER_HEIGHT) {
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

	if (e.x < 0 || e.y < 0 || e.x > RENDERER_WIDTH || e.y > RENDERER_HEIGHT) {
		return;
	}

	EntityManager* entity_mgr = EntityManager::GetInstance();

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
}

void GameManager::OnStarting() {
	ResetGameData();
}

void GameManager::ResetGameData() {
	this->score = 0;
	this->currentCombo = 0;
	this->remainingLives = 3;
}

void GameManager::AddScore(int score) {
	SetScore(this->score + score);
}

void GameManager::SetScore(int score) {
	this->score = score;
	if (this->score > this->gameData->highestScore) {
		this->gameData->highestScore = this->score;
	}
	EntityManager::GetInstance()->spawnTask->interval = max((int)(ENEMY_SPAWN_INTERVAL_BASE - this->score * ENEMY_SPAWN_INTERVAL_MULTIPLIER), ENEMY_SPAWN_INTERVAL_MAX);

	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);
	TextElement* scoreElement = (TextElement*)mainStage->GetElementById("score");

	scoreElement->SetText(to_string(this->score).c_str());
}

void GameManager::AddCombo(int combo) {
	SetCombo(currentCombo + combo);
}

void GameManager::SetCombo(int combo) {
	currentCombo = combo;

	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);
	TextElement* comboElement = (TextElement*)mainStage->GetElementById("combo");
	comboElement->SetText(to_string(currentCombo).c_str());

	if (currentCombo == 0) {
		comboExpirationTick = 0;
		comboElement->SetActive(false);
	}
	else {
		comboExpirationTick = max(COMBO_DURATION_BASE + currentCombo * COMBO_DURATION_MULTIPLIER, COMBO_DURATION_MAX);
		if (currentCombo > gameData->highestComboAchived) {
			gameData->highestComboAchived = currentCombo;
		}
		comboElement->SetActive(true);
	}
}