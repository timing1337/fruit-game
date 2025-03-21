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

	//Check for collision
	GameManager* gameManager = GameManager::getInstance();
	MousePathRecord* mousePathRecord = gameManager->mousePathRecord;
	vector<MousePath> points = mousePathRecord->paths;

	if (!mousePathRecord->isRecording) {
		return;
	}

	if (points.size() > 3) {
		vector<SDL_Point> linePoints = getLinePoints(points[points.size() - 1].point, points[points.size() - 2].point);

		for (auto& point : linePoints) {
			for (auto& entity : this->entities) {
				if (!entity.alive) continue;
				if (entity.IsCollidingWithPoint(point.x, point.y)) {
					entity.onHit();
				}
			}
		}
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

void Entity::despawn() {
	this->alive = false;
}

void Entity::onRender() {
	Renderer* renderer = Renderer::getInstance();
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);

	SDL_FRect rect = { position.x - 15, position.y - 15, 30, 30 };
	SDL_RenderFillRectF(renderer->gRenderer, &rect);
}

bool Entity::IsCollidingWithPoint(int x, int y) {
	return isPointInRect({x, y}, {(int)position.x - 15, (int)position.y - 15}, {30, 30});
}

void Entity::onHit() {
	this->despawn();

	GameManager::getInstance()->UpdateScore(1);
}
