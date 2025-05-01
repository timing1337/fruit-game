#include "enemy.h"

void Enemy::onHit() {
	this->despawn(EntityDeathType::PLAYER_INTERACTION);
	EntityManager::GetInstance()->spawnParticle(this->position);
}

void Enemy::onTick(int deltaTicks) {
	Entity::onTick(deltaTicks);
}

void Enemy::onDespawn(EntityDeathType type) {

	GameManager* game_mgr = GameManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();

	switch (type) {
	case EntityDeathType::OUT_OF_BOUND:
		break;
	case EntityDeathType::PLAYER_INTERACTION:
		game_mgr->AddScore(this->scoreBase);
		game_mgr->AddCombo(1);
		break;
	}
}


//this should be in the base class but because of circular dependencies, i have not been able to do so
//this is not a BIG issue but its annoying as f
//TODO: think of a way to move it to the base class
void Enemy::onRender() {
	Renderer* renderer = Renderer::GetInstance();
	if (entityTexture == nullptr) {
		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_Rect rect = { position.x - hitbox.x / 2, position.y - hitbox.y / 2, hitbox.x, hitbox.y };
		SDL_RenderFillRect(renderer->gRenderer, &rect);
	}
	else {
		renderer->RenderTexture(entityTexture, position.x, position.y, Alignment::CENTER);
	}
}