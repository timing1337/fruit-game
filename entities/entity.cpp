#include "entity.h"

Entity::Entity(EntityType type, vec2_t position, vec2_t direction): type(type), position(position), direction(direction){
	this->hitbox = vec2_t(0, 0);
};

Entity::Entity(EntityType type, vec2_t position, float speed, float angle): type(type){
	float radian = deg2rad(angle);
	this->position = position;
	this->direction = vec2_t(speed * cos(radian), speed * sin(radian));
	this->hitbox = vec2_t(0, 0);
};

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


//this is kinda bad? only works well if entity bounding box is square
bool Entity::IsColliding(int x, int y) {
	int halfBoundX = this->hitbox.x / 2;
	int halfBoundY = this->hitbox.y / 2;

	return x >= position.x - halfBoundX && x <= position.x + halfBoundX && y >= position.y - halfBoundY && y <= position.y + halfBoundY;
}

void Entity::onTick(int deltaTicks) {
	if (!this->alive) {
		return;
	}

	if (this->position.x < -50 || this->position.x > RENDERER_WIDTH) {//screen x bound
		this->despawn(EntityDeathType::OUT_OF_BOUND);
		return;
	}

	if (this->position.y < -50 || this->position.y > RENDERER_HEIGHT) { //screen y bound
		this->despawn(EntityDeathType::OUT_OF_BOUND);
		return;
	}

	float deltaSeconds = deltaTicks / 1000.0f;
	//Gravity

	//dont accerlerate gravity speed if we are froze
	if(this->slowdownFactor == 0){
		this->direction.y -= GRAVITY_SPEED * deltaSeconds;
	}

	this->position.x += this->direction.x * deltaSeconds * (1.0f - this->slowdownFactor);
	this->position.y -= this->direction.y * deltaSeconds * (1.0f - this->slowdownFactor);
}

void Entity::despawn(EntityDeathType type) {
	this->alive = false;
	onDespawn(type);
}

void Entity::onDespawn(EntityDeathType type) {
}