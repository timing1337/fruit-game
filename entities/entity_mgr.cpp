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
			//base rate 0.5 + 0.1 per 10 score
			float baseRate = 0.4 + game_mgr->score * 0.05;
			float rate = entity_mgr->distribution(entity_mgr->mt);
			if (rate > baseRate) {
				return;
			}
			SDL_Log("Spawning enemy");
			int direction = 0;
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

			entity_mgr->spawnEntity<Enemy>(position, speed, angle);
		},
		[](RepeatedTask* self) {
		}
	);
}

void EntityManager::Heartbeat(int deltaTicks) {
	//Only when the game is running otherwise
	for (int i = 0; i < this->entities.size(); i++) {
		Entity* entity = this->entities[i];
		if (!entity->alive) {
			delete entity;
			this->entities.erase(this->entities.begin() + i);
			continue;
		}

		entity->onTick(deltaTicks);
	}
}