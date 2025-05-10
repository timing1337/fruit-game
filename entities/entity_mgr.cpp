#include "entity_mgr.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::Initialize() {
	spawnTask = TaskManager::GetInstance()->RunRepeatedTask(1000,
		[](RepeatedTask* self) {
			EntityManager::GetInstance()->RandomizeSpawningEntity();
		}, [](RepeatedTask* self){});
}

void EntityManager::RandomizeSpawningEntity() {
	GameManager* game_mgr = GameManager::GetInstance();

	if (game_mgr->state != GameState::RUNNING) {
		return;
	}

	float spawnRate = max(ENEMY_SPAWN_BASE_RATE + game_mgr->score * ENEMY_SPAWN_RATE_MULTIPLIER, ENEMY_SPAWN_RATE_MAX);

	float rate = rand() % 100 / 100.0f;
	if (rate > spawnRate) {
		return;
	}

	int direction = rand() % 3;
	float speed = min(ENEMY_BASE_SPEED + game_mgr->score * ENEMY_SPEED_MULTIPLIER, ENEMY_SPEED_MAX);
	float angle = 0;
	vec2_t position;
	switch (direction) {
	case 0: //RIGHT
		position = vec2_t(0, (RENDERER_CENTER_Y - 50) + rand() % 200);
		angle = 20.0f;
		break;
	case 1: //LEFT
		position = vec2_t(RENDERER_WIDTH, (RENDERER_CENTER_Y - 50) + rand() % 200);
		angle = 150.0f;
		break;
	case 2: //BOTTOM
		position = vec2_t((RENDERER_CENTER_Y - 50) + rand() % 300, RENDERER_HEIGHT);
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
	
	SDL_Log("Spawned enemy at %f, %f", position.x, position.y);
	SDL_Log("Hp: %d", enemy->hp);

	spawnEntity(enemy);
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
		Particle* particle = new Particle(randomizedPosition, randomSpeed, randomAngle);
		particle->color = deathParticleColor;
		particle->SetHitbox({ 5, 5 });

		spawnEntity(particle);
	}
}