#include "enemy.h"

void Enemy::onHit() {
	this->hp -= 1;

	SDL_Log("Enemy hit! Remaining HP: %d", this->hp);
	if (this->hp <= 0) {
		EntityManager::GetInstance()->spawnParticle(this->position, deathParticleColor);
		this->despawn(EntityDeathType::PLAYER_INTERACTION);
	}
}

void Enemy::onTick(int deltaTicks) {
	Entity::onTick(deltaTicks);
}

void Enemy::onDespawn(EntityDeathType type) {

	GameManager* game_mgr = GameManager::GetInstance();
	Renderer* renderer = Renderer::GetInstance();

	switch (type) {
	case EntityDeathType::OUT_OF_BOUND:
		SDL_Log("Enemy out of bound");
		game_mgr->SetRemainingLives(game_mgr->remainingLives - 1);
		game_mgr->SetCombo(0);
		break;
	case EntityDeathType::PLAYER_INTERACTION:
		game_mgr->AddScore(this->score);
		game_mgr->AddCombo(1);

		game_mgr->slicedFruit++;
		break;
	}
}

void Enemy::onRender() {
	Renderer* renderer = Renderer::GetInstance();
	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);
	SDL_SetRenderTarget(renderer->gRenderer, mainStage->gameCanvas);
	if (entityTexture == nullptr) {
		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_Rect rect = { position.x - hitbox.x / 2, position.y - hitbox.y / 2, hitbox.x, hitbox.y };
		SDL_RenderFillRect(renderer->gRenderer, &rect);
	}
	else {
		renderer->RenderTexture(entityTexture, position.x, position.y, Alignment::CENTER, AlignmentVertical::MIDDLE);
	}
}