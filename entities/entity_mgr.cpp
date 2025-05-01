#include "entity_mgr.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::Initialize() {
	entity_random_mt.seed(time(0));

	entityDistribution = uniform_real_distribution<float>(0, 1);
	spawnDirection = uniform_int_distribution<int>(1, 3);
	xBound = uniform_int_distribution<int>(0, RENDERER_WIDTH);
	yBound = uniform_int_distribution<int>(100, RENDERER_HEIGHT - 100);

	particleRandomPosition = uniform_real_distribution<float>(-15, 15);
	particleRandomAngle = uniform_real_distribution<float>(20, 150);
	particleRandonSpeed = uniform_real_distribution<float>(200, 300);

	//TODO: only start this when the game is running & kill it when we get to post game status
	//honestly we can let it run throughout the game, just a premature optimization if we do as above

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

	float rate = entityDistribution(entity_random_mt);
	if (rate > spawnRate) {
		return;
	}

	int direction = spawnDirection(entity_random_mt);
	float speed = min(ENEMY_BASE_SPEED + game_mgr->score * ENEMY_SPEED_MULTIPLIER, ENEMY_SPEED_MAX);
	float angle = 0; // degree
	vec2_t position;
	switch (direction) {
	case 0: //RIGHT
		position = vec2_t(0, yBound(entity_random_mt));
		angle = 20.0f;
		break;
	case 1: //LEFT
		position = vec2_t(xBound.max(), yBound(entity_random_mt));
		angle = 150.0f;
		break;
	case 2: //BOTTOM
		position = vec2_t(xBound(entity_random_mt), yBound.max());
		angle = 90.0f;
		break;
	}

	Enemy* enemy = new Enemy(position, speed, angle);
	enemy->SetHitbox({ 20, 20 });
	spawnEntity(enemy);
}

void EntityManager::Heartbeat(int deltaTicks) {
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

void EntityManager::spawnParticle(vec2_t position) {
	for (int i = 0; i < 7; i++) {
		vec2_t randomizedPosition = vec2_t(position.x + particleRandomPosition(entity_random_mt), position.y + particleRandomPosition(entity_random_mt));
		Particle* particle = new Particle(randomizedPosition, particleRandonSpeed(entity_random_mt), particleRandomAngle(entity_random_mt));
		particle->SetHitbox({ 5, 5 });
		spawnEntity(particle);
	}
}