#include "particle.h"

void Particle::onTick(int deltaTicks) {
	this->aliveTicks += deltaTicks;
	if (this->aliveTicks >= this->maxAliveTicks) {
		this->despawn(EntityDeathType::UNKNOWN);
		return;
	}
	Entity::onTick(deltaTicks);
}

void Particle::onRender() {
	Renderer* renderer = Renderer::GetInstance();
	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);

	float progress = (float)this->aliveTicks / this->maxAliveTicks;
	int calculatedOpacity = max(0, (int)(255 - (progress * 255)));

	SDL_SetRenderTarget(renderer->gRenderer, mainStage->glowCanvas);
	SDL_SetRenderDrawColor(renderer->gRenderer, this->color.r, this->color.g, this->color.b, calculatedOpacity);

	SDL_Rect fillRect = { this->position.x - this->hitbox.x / 2, this->position.y - this->hitbox.y / 2, this->hitbox.x, this->hitbox.y };
	SDL_RenderFillRect(renderer->gRenderer, &fillRect);
}

void Particle::onDespawn(EntityDeathType type) {}

void Particle::onHit() {}