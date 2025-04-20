#include "particle.h"

void Particle::onTick(int deltaTicks) {
	this->aliveTicks += deltaTicks;
	if (this->aliveTicks > 1000) {
		SDL_Log("Particle despawned due to time limit");
		this->despawn(EntityDeathType::UNKNOWN);
		return;
	}
	Entity::onTick(deltaTicks);
}

void Particle::onRender() {
	Renderer* renderer = Renderer::getInstance();
	int calculatedOpacity = max(255 - ((this->aliveTicks * 255) / 1000), 0);
	SDL_Rect fillRect = { (int)this->position.x - (int)this->hitbox.x / 2, (int)this->position.y - (int)this->hitbox.y / 2, (int)this->hitbox.x, (int)this->hitbox.y };
	SDL_FillRect(MainScene::gameCanvas, &fillRect, (0xFF << 24) | (0xFF << 16) | (0xFF << 8) | calculatedOpacity);
}

void Particle::onDespawn(EntityDeathType type) {}

void Particle::onHit() {}