#include "entity.h"

void Entity::onTick(int deltaTicks) {
	if (!this->alive) {
		return;
	}

	if (this->position.x < -50 || this->position.x > 1490) {//screen y bound
		this->despawn();
		return;
	}

	if (this->position.y < -50 || this->position.y > 860) { //screen x bound
		this->despawn();
		return;
	}

	float deltaSeconds = deltaTicks / 1000.0f;

	if (this->direction.x <= 0) {
		this->direction.x = 0;
	}
	else {
		//Air resistance
		this->direction.x -= 10 * deltaSeconds;
	}

	//Gravity
	this->direction.y -= 10 * deltaSeconds;

	this->position.x += this->direction.x * deltaSeconds;
	this->position.y -= this->direction.y * deltaSeconds;

	this->position.y += 10 * deltaSeconds;
}

void Entity::despawn(EntityDeathType type) {
	this->alive = false;
	onDespawn(type);
}

void Entity::onDespawn(EntityDeathType type){
}