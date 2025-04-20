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
	int color = (0xFF << 24) | (0xFF << 16) | (0xFF << 8) | calculatedOpacity;

	//first pass

	int hitboxX = this->hitbox.x;
	int hitboxY = this->hitbox.y;

	SDL_Rect fillRect = { this->position.x - hitboxX / 2, this->position.y - this->hitbox.y / 2, hitboxX, this->hitbox.y };
	SDL_FillRect(MainScene::gameCanvas, &fillRect, color);

	int upscaledHitboxX = this->hitbox.x * 1.5f;
	int upscaledHitboxY = this->hitbox.y * 1.5f;
	SDL_Rect downsampledFillRect = { this->position.x - upscaledHitboxX / 2, this->position.y - upscaledHitboxY / 2, upscaledHitboxX, upscaledHitboxY};

	SDL_FillRect(MainScene::downsampledGameCanvas, &downsampledFillRect, color);
}

void Particle::onDespawn(EntityDeathType type) {}

void Particle::onHit() {}