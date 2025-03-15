#include "entity.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::spawnEntity(vec2_t position, vec2_t direction) {
	Entity entity;
	entity.position = position;
	entity.direction = direction;
	this->entities.push_back(entity);
}

void EntityManager::spawnEntity(vec2_t position, float degree, float speed) {
	float radian = deg2rad(degree);
	EntityManager::spawnEntity(position, vec2_t(cos(radian) * speed, sin(radian) * speed));
}

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

void Entity::onTick() {
	float deltaTime = GameManager::getInstance()->deltaTime;

	this->direction.y -= 10 * deltaTime;

    this->position.x += this->direction.x * deltaTime;
    this->position.y -= this->direction.y * deltaTime;
	this->position.y += 30 * deltaTime; //gravity
}