#include "game/game.h"

GameManager* GameManager::instancePtr = new GameManager();

void GameManager::Heartbeat(int deltaTicks) {
	if (this->state != GameState::RUNNING) {
		return;
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

	this->mousePathRecord->AddPoint({ e.x, e.y });

	for (int i = 0; i < this->mousePathRecord->paths.size() - 1; i++) {
		MousePath* path = &this->mousePathRecord->paths[i];
		if (path->longevity <= 350) {
			continue;
		}
		SDL_Point lastPoint = path->point;
		SDL_Point currentPoint = this->mousePathRecord->paths[i + 1].point;
		std::vector<SDL_Point> points = Algorithm_GetPoints(lastPoint, currentPoint, 1);

		for (int i = 0; i < points.size(); i++) {
			SDL_Point point = points[i];
			for (int i = 0; i < entity_mgr->entities.size(); i++) {
				Entity* entity = entity_mgr->entities[i];
				if (!entity->alive) continue;
				if (entity->IsColliding(point.x, point.y)) {
					entity->onHit();
				}
			}
		}
	}
}

void GameManager::TriggerBuff(BuffConfig* config) {
	if (this->activeBuff != BUFF_NONE) return;
	this->activeBuff = config->id;

	TaskManager* task_mgr = TaskManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();
	EntityManager* entity_mgr = EntityManager::GetInstance();

	switch (config->id) {
	case BuffId::FREEZE: {
		//goofy
		// Fade-in effect
		task_mgr->RunTimerTask(500,
			//oh my god this is so ulgy
			[renderer, entity_mgr](TimerTask* self) {
				int opacity = static_cast<int>(self->GetProgress() * 255);
				renderer->RenderTextureBackground("freeze_buff_overlay.png", opacity);

				for (Entity* entity : entity_mgr->entities) {
					if (!entity->alive) continue;
					entity->slowdownFactor = std::max(self->GetProgress(), 1.0f);
				}
			},
			[this, config, task_mgr, renderer, entity_mgr](TimerTask* self) {
				// Active phase
				task_mgr->RunTimerTask(config->duration,
					[renderer](TimerTask* self) {
						renderer->RenderTextureBackground("freeze_buff_overlay.png", 255);
					},
					[this, task_mgr, renderer, entity_mgr](TimerTask* self) {
						// Fade-out effect
						task_mgr->RunTimerTask(500,
							[renderer, entity_mgr](TimerTask* self) {
								int opacity = static_cast<int>(255 - self->GetProgress() * 255);
								renderer->RenderTextureBackground("freeze_buff_overlay.png", opacity);

								for (Entity* entity : entity_mgr->entities) {
									if (!entity->alive) continue;
									entity->slowdownFactor = 0;
								}
							},
							[this](TimerTask* self) {
								this->activeBuff = BUFF_NONE;
								EntityManager::GetInstance()->canSpawnBuff = true;
							});
					});
			});
		break;
	}
	case BuffId::DOUBLE_SCORE:
		this->activeBuff = BUFF_NONE;
		break;

	case BuffId::FRUIT_PARTY:
		entity_mgr->spawnTask->interval = 30;
		task_mgr->RunTimerTask(config->duration,
			[this](TimerTask* self) {
			},
			[this, entity_mgr](TimerTask* self) {
				//kill every remaining entity & halt the spawn process
				entity_mgr->spawnTask->interval = 9999;
				for (int i = 0; i < entity_mgr->entities.size(); i++) {
					Entity* entity = entity_mgr->entities[i];
					if (!entity->alive || entity->type != EntityType::ENEMY) continue;
					Enemy* enemy = (Enemy*)entity;
					enemy->despawn(EntityDeathType::OUT_OF_BOUND);
					entity_mgr->spawnParticle(entity->position, enemy->deathParticleColor);
				}

				//7 second cooldown before buff end
				TaskManager::GetInstance()->RunTimerTask(500,
					[](TimerTask* self) {},
					[this, entity_mgr](TimerTask* self) {
						entity_mgr->spawnTask->interval = std::max(ENEMY_SPAWN_INTERVAL_BASE - this->score * ENEMY_SPAWN_INTERVAL_MULTIPLIER, ENEMY_SPAWN_INTERVAL_MIN);
						this->activeBuff = BUFF_NONE;
					});
			});
		break;
	}
}

void GameManager::FireStateChange(GameState state) {
	this->state = state;
	switch (state) {
	case GameState::WAITING:
		//Play music again
		AudioManager::GetInstance()->PlayMusic();
		break;
	case GameState::STARTING:
		AudioManager::GetInstance()->HaltMusic();
		ResetRuntimeGameData();
		break;
	case GameState::PAUSED:
		//Pause the game and cancel the current path if player is slashing
		if (this->mousePathRecord != nullptr) {
			this->mousePathRecordsLeftover.push_back(this->mousePathRecord);
			this->mousePathRecord = nullptr;
		}
		break;
	case GameState::POSTGAME:
		if (this->highestComboRecorded > this->gameData->highestComboAchieved) {
			this->gameData->highestComboAchieved = this->highestComboRecorded;
		}

		if (this->score > this->gameData->highestScore) {
			this->gameData->highestScore = this->score;
		}

		this->gameData->ReloadBladeData();
		this->gameData->Save();
		break;
	case GameState::ENDGAME:
		EntityManager* entity_mgr = EntityManager::GetInstance();
		//clear out old entities and mouse path
		for (int i = 0; i < this->mousePathRecordsLeftover.size(); i++) {
			MousePathRecord* record = this->mousePathRecordsLeftover[i];
			delete record;
		}
		this->mousePathRecordsLeftover.clear();

		delete this->mousePathRecord;
		this->mousePathRecord = nullptr;

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
	MainMenu* endStage = (MainMenu*)scene_mgr->GetScene(SceneId::END_GAME);
	TaskManager* task_mgr = TaskManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();

	if (this->remainingLives == 0) {
		GameManager::GetInstance()->FireStateChange(GameState::POSTGAME);
		TaskManager::GetInstance()->RunTimerTask(FADING_OUT_TRANSITION_TICKS,
			[renderer](TimerTask* self) {
				renderer->SetBackgroundColor(255, 0, 0, self->GetProgress() * 255);
			}, [renderer, mainStage, endStage](TimerTask* self) {

				endStage->SetActive(true);
				mainStage->SetActive(false);

				TaskManager::GetInstance()->RunTimerTask(FADING_IN_TRANSITION_TICKS,
					[renderer](TimerTask* self) {
						renderer->SetBackgroundColor(255, 0, 0, 255 - (self->GetProgress() * 255));
					},
					[](TimerTask* self) {
						GameManager::GetInstance()->FireStateChange(GameState::ENDGAME);
					});
				});

		return;
	}

	int amount = MAX_LIVES - this->remainingLives;
	if (amount != 0) {
		int calculatedRedOpacity = amount * 30;
		task_mgr->RunTimerTask(150,
			[renderer, calculatedRedOpacity, mainStage](TimerTask* self) {
				renderer->SetBackgroundColor(255, 0, 0, self->GetProgress() * calculatedRedOpacity);
			},
			[mainStage, calculatedRedOpacity](TimerTask* self) {
				mainStage->redColorOverlayOpacity = calculatedRedOpacity;
			});
	}
}

void GameManager::ResetRuntimeGameData() {
	SetScore(0);
	SetCombo(0);
	SetRemainingLives(MAX_LIVES);
	this->highestComboRecorded = 0;
	this->slicedFruit = 0;
}

void GameManager::AddScore(int score) {
	SetScore(this->score + score);
}

void GameManager::SetScore(int score) {
	this->score = score;
	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);

	if (this->score > this->gameData->highestScore) {
		//dont save the score yet, just update the text
		TextElement* highestScoreElement = (TextElement*)mainStage->GetElementById("highest_score");
		highestScoreElement->SetText("Record: " + std::to_string(this->score));
	}

	TextElement* scoreElement = (TextElement*)mainStage->GetElementById("score");
	scoreElement->SetText(std::to_string(this->score));

	if (this->activeBuff != BuffId::FRUIT_PARTY) {
		EntityManager::GetInstance()->spawnTask->interval = std::max(ENEMY_SPAWN_INTERVAL_BASE - this->score * ENEMY_SPAWN_INTERVAL_MULTIPLIER, ENEMY_SPAWN_INTERVAL_MIN);
	}
}

void GameManager::AddCombo(int combo) {
	SetCombo(currentCombo + combo);
}

void GameManager::SetCombo(int combo) {
	currentCombo = combo;

	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);
	TextElement* comboElement = (TextElement*)mainStage->GetElementById("combo");
	comboElement->SetText(std::to_string(currentCombo));

	if (currentCombo == 0) {
		comboExpirationTick = 0;
		comboElement->SetActive(false);
	}
	else {
		comboExpirationTick = std::min(COMBO_DURATION_BASE + currentCombo * COMBO_DURATION_MULTIPLIER, COMBO_DURATION_MAX);
		comboElement->SetActive(true);

		this->highestComboRecorded = std::max(this->highestComboRecorded, currentCombo);
	}
}