#include "entity.h"

void Entity::onTick() {
	if (!this->alive) {
		return;
	}

	if (this->position.y < -50 || this->position.y > Renderer::getInstance()->width + 50) {
		this->despawn();
		return;
	}

	if (this->position.x < -50 || this->position.x > Renderer::getInstance()->width + 50) {
		this->despawn();
		return;
	}

	float deltaTime = GameManager::getInstance()->deltaTime / 1000.0f;

	if (this->direction.x <= 0) {
		this->direction.x = 0;
	}
	else {
		//Air resistance
		this->direction.x -= 10 * deltaTime;
	}

	//Gravity
	this->direction.y -= 10 * deltaTime;

	this->position.x += this->direction.x * deltaTime;
	this->position.y -= this->direction.y * deltaTime;

	this->position.y += 10 * deltaTime;
}

void Entity::despawn(EntityDeathType type) {
	this->alive = false;
	onDespawn(type);
}

void Entity::onDespawn(EntityDeathType type){
}