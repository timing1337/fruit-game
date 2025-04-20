#include "entity_mgr.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::Initialize() {
	EntityManager* entity_mgr = EntityManager::getInstance();
	GameManager* game_mgr = GameManager::getInstance();
	entity_mgr->mt.seed(time(0));
	entity_mgr->distribution = uniform_real_distribution<float>(0, 1);
	entity_mgr->spawnDirection = uniform_int_distribution<int>(1, 3);
	entity_mgr->xBound = uniform_int_distribution<int>(0, Renderer::getInstance()->width);
	entity_mgr->yBound = uniform_int_distribution<int>(200, Renderer::getInstance()->height - 100);
	entity_mgr->spawnTask = TaskManager::getInstance()->RunRepeatedTask(1000,
		[entity_mgr, game_mgr](RepeatedTask* self) {
			if (game_mgr->state != GameState::RUNNING) return;
			//spawn successful rate
			//base rate 0.4 + 0.03 per 1 score
			float baseRate = 0.4 + game_mgr->score * 0.03;
			float rate = entity_mgr->distribution(entity_mgr->mt);
			if (rate > baseRate) {
				return;
			}
			SDL_Log("Spawning enemy");
			int direction = entity_mgr->spawnDirection(entity_mgr->mt);
			float speed = 200 + game_mgr->score * 5;
			float angle = 0;
			vec2_t position;
			switch (direction) {
			case 0: //RIGHT
				position = vec2_t(0, entity_mgr->yBound(entity_mgr->mt));
				angle = 20;
				break;
			case 1: //LEFT
				position = vec2_t(entity_mgr->xBound.max(), entity_mgr->yBound(entity_mgr->mt));
				angle = 150;
				break;
			case 2: //BOTTOM
				position = vec2_t(entity_mgr->xBound(entity_mgr->mt), entity_mgr->yBound.max());
				angle = 90;
				break;
			}

			Enemy* enemy = new Enemy(position, speed, angle);
			enemy->SetHitbox({ 20, 20 });
			entity_mgr->spawnEntity(enemy);
		},
		[](RepeatedTask* self) {
		}
	);
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

	uniform_real_distribution<float> randomX(-15, 15);
	uniform_real_distribution<float> randomY(-15, 15);
	uniform_real_distribution<float> randomShootingAngle(20, 150);
	uniform_real_distribution<float> randomSpeed(200, 300);
	for (int i = 0; i < 7; i++) {
		vec2_t randomizedPosition = vec2_t(position.x + randomX(mt), position.y + randomX(mt));
		Particle* particle = new Particle(randomizedPosition, randomSpeed(mt), randomShootingAngle(mt));
		particle->SetHitbox({ 5, 5 });
		spawnEntity(particle);
	}
}