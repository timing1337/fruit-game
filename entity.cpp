#include "entity.h"

EntityManager* EntityManager::instancePtr = new EntityManager();

void EntityManager::spawnEntity(vec2_t position, vec2_t direction, vec2_t rotation) {
	Entity entity;
	entity.position = position;
	entity.direction = direction;
	this->entities.push_back(entity);
}

void EntityManager::spawnEntity(vec2_t position, float velocityAngle, float speed, vec2_t rotation) {
	float radian = deg2rad(velocityAngle);
	EntityManager::spawnEntity(position, vec2_t(cos(radian) * speed, sin(radian) * speed), rotation);
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

	this->direction.y -= 10 * deltaTime;
	this->direction.x -= 10 * deltaTime;

    this->position.x += this->direction.x * deltaTime;
    this->position.y -= this->direction.y * deltaTime;
	this->position.y += 10 * deltaTime; //gravity
}

void Entity::despawn() {
	SDL_Log("Despawn");
	this->alive = false;
}

void Entity::onRender() {
	Renderer* renderer = Renderer::getInstance();
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);

	SDL_FRect rect = { position.x - 15, position.y - 15, 30, 30 };
	SDL_RenderFillRectF(renderer->gRenderer, &rect);
}