#include "particle.h"

void Particle::onTick(int deltaTicks) {
	this->aliveTicks += deltaTicks;
	if (this->aliveTicks >= PARTICLE_ALIVE_TICK) {
		this->despawn(EntityDeathType::UNKNOWN);
		return;
	}
	Entity::onTick(deltaTicks);
}

void Particle::onRender() {
	Renderer* renderer = Renderer::GetInstance();
	float progress = (float)this->aliveTicks / PARTICLE_ALIVE_TICK;
	int calculatedOpacity = max(0, (int)(255 - (progress * 255)));
	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);

	int color = SDL_MapRGBA(mainStage->gameCanvas->format, this->color.r, this->color.g, this->color.b, calculatedOpacity);

	//first pass

	int hitboxX = this->hitbox.x;
	int hitboxY = this->hitbox.y;
	SDL_Rect fillRect = { this->position.x - hitboxX / 2, this->position.y - this->hitbox.y / 2, hitboxX, this->hitbox.y };
	SDL_FillRect(mainStage->gameCanvas, &fillRect, color);
}

void Particle::onDespawn(EntityDeathType type) {}

void Particle::onHit() {}