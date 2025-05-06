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
	switch (state) {
	case GameState::WAITING:
		break;
	case GameState::STARTING:
		ResetGameData();
		break;
	case GameState::POSTGAME:
		this->gameData->Save();
		break;
	case GameState::ENDGAME:
		//clear out old entities and mouse path
		for (int i = 0; i < this->mousePathRecordsLeftover.size(); i++) {
			MousePathRecord* record = this->mousePathRecordsLeftover[i];
			delete record;
		}
		this->mousePathRecordsLeftover.clear();

		delete this->mousePathRecord;
		this->mousePathRecord = nullptr;

		EntityManager* entity_mgr = EntityManager::GetInstance();

		for (int i = 0; i < entity_mgr->entities.size(); i++) {
			Entity* entity = entity_mgr->entities[i];
			if (entity->alive) {
				delete entity;
			}
		}
		entity_mgr->entities.clear();
		break;
	}
}

void GameManager::SetRemainingLives(int lives) {
	if (lives < 0) return;
	SDL_Log("Remaining lives: %d", lives);
	SceneManager* scene_mgr = SceneManager::GetInstance();
	this->remainingLives = lives;
	MainStage* mainStage = (MainStage*)scene_mgr->GetScene(SceneId::GAME);
	MainMenu* mainMenu = (MainMenu*)scene_mgr->GetScene(SceneId::MAIN_MENU);
	TaskManager* task_mgr = TaskManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();

	if (this->remainingLives == 0) {
		GameManager::GetInstance()->FireStateChange(GameState::POSTGAME);
		TaskManager::GetInstance()->RunTimerTask(FADING_OUT_TRANSITION_TICKS,
			[renderer](TimerTask* self) {
				renderer->SetBackgroundColor(255, 0, 0, self->GetProgress() * 255);
			}, [renderer, mainStage, mainMenu](TimerTask* self) {
				mainStage->redColorOverlayOpacity = 0;
				mainStage->shakeIntensity = 0;
				mainStage->shakeFrequency = 0;

				mainMenu->SetActive(true);
				mainStage->SetActive(false);

				TaskManager::GetInstance()->RunTimerTask(FADING_IN_TRANSITION_TICKS,
					[renderer](TimerTask* self) {
						renderer->SetBackgroundColor(255, 0, 0, 255 - (self->GetProgress() * 255));
					},
					[](TimerTask* self) {
						GameManager::GetInstance()->FireStateChange(GameState::ENDGAME);
						GameManager::GetInstance()->FireStateChange(GameState::WAITING);
					});
				});

		return;
	}

	int amount = MAX_LIVES - this->remainingLives;
	if (amount != 0) {
		int calculatedRedOpacity = amount * 30;
		int shakeIntensity = amount * 3;
		mainStage->shakeFrequency = 0.2f + (amount * 0.1f);
		mainStage->shakeIntensity = shakeIntensity;

		task_mgr->RunTimerTask(150,
			[renderer, calculatedRedOpacity, mainStage](TimerTask* self) {
				renderer->SetBackgroundColor(255, 0, 0, self->GetProgress() * calculatedRedOpacity);
			},
			[mainStage, calculatedRedOpacity](TimerTask* self) {
				mainStage->redColorOverlayOpacity = calculatedRedOpacity;
			});
	}
}

void GameManager::ResetGameData() {
	SetScore(0);
	SetCombo(0);
	SetRemainingLives(MAX_LIVES);
}

void GameManager::AddScore(int score) {
	SetScore(this->score + score);
}

void GameManager::SetScore(int score) {
	this->score = score;
	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);

	if (this->score > this->gameData->highestScore) {
		this->gameData->highestScore = this->score;

		TextElement* highestScoreElement = (TextElement*)mainStage->GetElementById("highest_score");
		highestScoreElement->SetText("Record: " + to_string(this->score));
	}
	TextElement* scoreElement = (TextElement*)mainStage->GetElementById("score");
	scoreElement->SetText(to_string(this->score));

	EntityManager::GetInstance()->spawnTask->interval = max((int)(ENEMY_SPAWN_INTERVAL_BASE - this->score * ENEMY_SPAWN_INTERVAL_MULTIPLIER), ENEMY_SPAWN_INTERVAL_MAX);
}

void GameManager::AddCombo(int combo) {
	SetCombo(currentCombo + combo);
}

void GameManager::SetCombo(int combo) {
	currentCombo = combo;

	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);
	TextElement* comboElement = (TextElement*)mainStage->GetElementById("combo");
	comboElement->SetText(to_string(currentCombo));

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