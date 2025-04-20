#include "entity.h"

Entity::Entity(vec2_t position, vec2_t direction, vec2_t rotation) {
	this->position = position;
	this->direction = direction;
	this->rotation = rotation;
	this->hitbox = vec2_t(0, 0);
};

Entity::Entity(vec2_t position, float speed, float angle, vec2_t rotation) {
	float radian = deg2rad(angle);
	this->position = position;
	this->direction = vec2_t(speed * cos(radian), speed * sin(radian));
	this->rotation = rotation;
	this->hitbox = vec2_t(0, 0);
};

//we only allow custom hitbox for non-textured entities
void Entity::SetHitbox(vec2_t hitbox) {
	if (this->entityTexture == nullptr) {
		this->hitbox = hitbox;
	}
}

void Entity::SetTexture(GameTexture* texture) {
	this->entityTexture = texture;

	//recalculate hitbox

	this->hitbox.x = this->entityTexture->width;
	this->hitbox.y = this->entityTexture->height;
}

bool Entity::IsColliding(int x, int y) {
	return isPointInRect({ x, y }, { (int)this->position.x - (int)this->hitbox.x / 2, (int)this->position.y - (int)this->hitbox.y / 2 }, { (int)this->hitbox.x, (int)this->hitbox.y });
}

void Entity::onTick(int deltaTicks) {
	if (!this->alive) {
		return;
	}

	if (this->position.x < -50 || this->position.x > 1490) {//screen y bound
		this->despawn(EntityDeathType::OUT_OF_BOUND);
		return;
	}

	if (this->position.y < -50 || this->position.y > 860) { //screen x bound
		this->despawn(EntityDeathType::OUT_OF_BOUND);
		return;
	}

	float deltaSeconds = deltaTicks / 1000.0f;

	//Gravity
	this->direction.y -= 250 * deltaSeconds;

	this->position.x += this->direction.x * deltaSeconds;
	this->position.y -= this->direction.y * deltaSeconds;
}

void Entity::despawn(EntityDeathType type) {
	this->alive = false;
	onDespawn(type);
}

void Entity::onDespawn(EntityDeathType type) {
}