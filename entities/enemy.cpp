#include "enemy.h"

void Enemy::onHit() {
	this->hp -= 1;

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
		if (game_mgr->activeBuff == BuffId::FRUIT_PARTY) {
			return;
		}
		game_mgr->SetRemainingLives(game_mgr->remainingLives - 1);
		game_mgr->SetCombo(0);
		break;
	case EntityDeathType::PLAYER_INTERACTION:
		game_mgr->AddScore(this->score);
		game_mgr->AddCombo(1);

		game_mgr->slicedFruit++;

		if (this->buff != nullptr) {
			game_mgr->TriggerBuff(this->buff);
		}
		break;
	}
}

void Enemy::onRender() {
	Renderer* renderer = Renderer::GetInstance();
	MainStage* mainStage = (MainStage*)SceneManager::GetInstance()->GetScene(SceneId::GAME);
	if (this->buff != nullptr) {
		SDL_SetRenderTarget(renderer->gRenderer, mainStage->glowCanvas);
		SDL_SetTextureColorMod(entityTexture->sprite->texture, this->buff->fruitColorChange.r, this->buff->fruitColorChange.g, this->buff->fruitColorChange.b);
		renderer->RenderTexture(entityTexture, position.x, position.y, Alignment::CENTER, AlignmentVertical::MIDDLE);
		SDL_SetTextureColorMod(entityTexture->sprite->texture, 255, 255, 255);
	}
	else {
		SDL_SetRenderTarget(renderer->gRenderer, mainStage->gameCanvas);
		renderer->RenderTexture(entityTexture, position.x, position.y, Alignment::CENTER, AlignmentVertical::MIDDLE);
	}
}