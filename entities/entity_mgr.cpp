#include "entity_mgr.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::Heartbeat() {
	for (int i = 0; i < this->entities.size(); i++) {
		Entity* entity = &this->entities[i];
		if (!entity->alive) {
			this->entities.erase(this->entities.begin() + i);
			continue;
		}
		entity->onTick();
	}
}

