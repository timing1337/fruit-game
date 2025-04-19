#include "shard.h"

void Shard::onTick(int deltaTicks) {
	Entity::onTick(deltaTicks);
	this->aliveTicks += deltaTicks;
	if (this->aliveTicks > 1000) {
		this->despawn(EntityDeathType::OUT_OF_BOUND);
	}
}

void Shard::onRender() {
	Renderer* renderer = Renderer::getInstance();
	int calculatedOpacity = max(100 - ((this->aliveTicks * 100) / 1000), 0);
	SDL_SetRenderDrawColor(Renderer::getInstance()->gRenderer, 255, 255, 255, calculatedOpacity);
	SDL_Rect fillRect = { (int)this->position.x - (int)this->hitbox.x / 2, (int)this->position.y - (int)this->hitbox.y / 2, (int)this->hitbox.x, (int)this->hitbox.y };
	SDL_RenderFillRect(Renderer::getInstance()->gRenderer, &fillRect);
}