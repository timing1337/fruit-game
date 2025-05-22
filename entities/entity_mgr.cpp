#include "entity_mgr.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::Initialize() {
	spawnTask = TaskManager::GetInstance()->RunRepeatedTask(ENEMY_SPAWN_INTERVAL_BASE,
		[](RepeatedTask* self) {
			EntityManager::GetInstance()->RandomizeSpawningEntity();
			GameManager* game_mgr = GameManager::GetInstance();

			if (game_mgr->activeBuff == BUFF_NONE) {
				float multiplySpawnChance = std::min(ENEMY_SPAWN_DUPLICATE_CHANCE_BASE + GameManager::GetInstance()->score * ENEMY_SPAWN_DUPLICATE_CHANCE_MULTIPLIER, ENEMY_SPAWN_DUPLICATE_CHANCE_MAX);
				float spawnChance = rand() % 100 / 100.0f;
				if (spawnChance < multiplySpawnChance) {
					int duplicateCount = rand() % 2 + 1;
					for (int i = 0; i < duplicateCount; i++) {
						TaskManager::GetInstance()->RunTimerTask(100 + i * 300, [](TimerTask* self) {}, [](TimerTask* self) {
							EntityManager::GetInstance()->RandomizeSpawningEntity();
						});
					}
				}
			}

		}, [](RepeatedTask* self){});
}

void EntityManager::RandomizeSpawningEntity() {
	GameManager* game_mgr = GameManager::GetInstance();
	TaskManager* task_mgr = TaskManager::GetInstance();
	AudioManager* audio_mgr = AudioManager::GetInstance();

	if (game_mgr->state != GameState::RUNNING) {
		return;
	}

	int direction = rand() % 3;
	float speed = std::min(ENEMY_BASE_SPEED + game_mgr->score * ENEMY_SPEED_MULTIPLIER, ENEMY_SPEED_MAX);
	float angle = 0;
	vec2_t position;
	switch (direction) {
	case 0: //RIGHT
		position = vec2_t(40, (RENDERER_CENTER_Y - 50) + rand() % 200);
		angle = 20.0f;
		break;
	case 1: //LEFT
		position = vec2_t(RENDERER_WIDTH - 40, (RENDERER_CENTER_Y - 50) + rand() % 200);
		angle = 150.0f;
		break;
	case 2: //BOTTOM
		position = vec2_t((RENDERER_CENTER_X - 50) + rand() % 300, RENDERER_HEIGHT);
		angle = 90.0f;
		break;
	}

	FruitConfig fruitConfig = FruitData::GetRandomFruitConfig();
	GameTexture* texture = Renderer::GetInstance()->GetTextureByName(fruitConfig.texture);

	Enemy* enemy = new Enemy(position, speed, angle);
	enemy->SetTexture(texture);
	enemy->score = fruitConfig.score;
	enemy->deathParticleColor = fruitConfig.color;
	enemy->hp = fruitConfig.maxHp;
	enemy->soundHit = fruitConfig.hitSound;
	enemy->buff = BuffData::GetBuffConfigById(FREEZE);

	audio_mgr->PlaySound("fruit_throw.wav");

	if (canSpawnBuff && game_mgr->activeBuff == BUFF_NONE) {
		float buffRate = rand() % 100 / 100.0f;
		float buffChance = std::min(0.005f + game_mgr->currentCombo * 0.0005f, 0.025f);
		if (buffRate < buffChance) {
			BuffConfig* buffConfig = BuffData::GetRandomBuffConfig();
			canSpawnBuff = false;
			enemy->buff = buffConfig;
		}
	}

	spawnEntity(enemy);

	if (game_mgr->activeBuff == FREEZE) {
		//apply slowest
		task_mgr->RunTimerTask(1000,
			//oh my god this is so ulgy
			[enemy, game_mgr](TimerTask* self) {
				//check if buff is still active and entity is still alive
				if (!enemy->alive) {
					self->Kill();
					return;
				}

				if (game_mgr->activeBuff != FREEZE) {
					self->Kill();
					enemy->slowdownFactor = 0.0f;
					return;
				}

				enemy->slowdownFactor = self->GetProgress();
			}, [](TimerTask* self) {});
	}
}

void EntityManager::Heartbeat(int deltaTicks) {
	GameManager* game_mgr = GameManager::GetInstance();
	if (game_mgr->state != GameState::RUNNING) {
		return;
	}
	for (int i = 0; i < this->entities.size(); i++) {
		Entity* entity = this->entities[i];
		if (!entity->alive) {
			continue;
		}
		entity->onTick(deltaTicks);
	}
}

void EntityManager::CleanUp() {
	for (int i = 0; i < this->entities.size(); i++) {
		Entity* entity = this->entities[i];
		if (!entity->alive) {
			if (entity->type == EntityType::ENEMY) {
				Enemy* enemy = (Enemy*)entity;
				if (enemy->buff != nullptr) {
					this->canSpawnBuff = true;
				}
			}
			delete entity;
			this->entities.erase(this->entities.begin() + i);
		}
	}
}

void EntityManager::spawnParticle(vec2_t position, SDL_Color deathParticleColor) {
	for (int i = 0; i < 7; i++) {
		int randomX = (rand() % 31) - 15;
		int randomY = (rand() % 31) - 15;
		int randomSpeed = 200 + rand() % 100;
		int randomAngle = 20 + (rand() % 130);
		vec2_t randomizedPosition = vec2_t(position.x + randomX, position.y + randomY);
		Particle* particle = new Particle(randomizedPosition, randomSpeed, randomAngle, PARTICLE_ALIVE_TICK);
		particle->color = deathParticleColor;
		particle->SetHitbox({ 5, 5 });

		spawnEntity(particle);
	}
}