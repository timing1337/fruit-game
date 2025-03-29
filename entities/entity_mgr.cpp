#include "entity_mgr.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::Heartbeat(int deltaTicks) {
	//Only when the game is running otherwise
	for (int i = 0; i < this->entities.size(); i++) {
		Entity* entity = this->entities[i];
		if (!entity->alive) {
			delete this->entities[i];
			this->entities.erase(this->entities.begin() + i);
			continue;
		}

		entity->onTick(deltaTicks);
	}

}