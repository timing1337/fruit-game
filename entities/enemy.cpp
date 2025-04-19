#include "enemy.h"

void Enemy::onHit() {
	this->despawn(EntityDeathType::PLAYER_INTERACTION);
}

void Enemy::onTick(int deltaTicks) {
	Entity::onTick(deltaTicks);
}

void Enemy::onDespawn(EntityDeathType type) {
	switch (type) {
	case EntityDeathType::OUT_OF_BOUND:
		if (GameManager::getInstance()->state != GameState::RUNNING) return;
		SDL_Log("Enemy despawned due to out of bound. Reducing lives");
		GameManager::getInstance()->remainingLives--;
		if (GameManager::getInstance()->remainingLives == 0) {
			SDL_Log("Triggering post game");
			Renderer* renderer = Renderer::getInstance();
			GameManager::getInstance()->FireStateChange(GameState::POSTGAME);
			renderer->PlayFadeTransition([this](BaseTask* self) {
				GameManager::getInstance()->FireStateChange(GameState::ENDGAME);
				SDL_Log("Game ended");
			}, [this](BaseTask* self) {});
		}
		break;
		//Reduce player life
	case EntityDeathType::PLAYER_INTERACTION:
		GameManager::getInstance()->AddScore(this->scoreBase);
		Renderer* renderer = Renderer::getInstance();

		TaskManager::getInstance()->RunTimerTask(400,
			[renderer](TimerTask* self) {
				SDL_Rect fillRect = { 0, 0, renderer->width, renderer->height };
				int calculatedOpacity = max(100 - ((self->counter * 100) / self->duration), 0);
				SDL_SetRenderDrawColor(renderer->gRenderer, 127, 250, 160, calculatedOpacity);
				SDL_RenderFillRect(renderer->gRenderer, &fillRect);
			}, [](BaseTask* self) {});


		break;
	}
}


//this should be in the base class but because of circular dependencies, i have not been able to do so
//this is not a BIG issue but its annoying as f
//TODO: think of a way to move it to the base class

void Enemy::onRender() {
	Renderer* renderer = Renderer::getInstance();
	if (entityTexture == nullptr) {
		SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);
		SDL_FRect rect = { position.x - hitbox.x / 2, position.y - hitbox.y / 2, hitbox.x, hitbox.y };
		SDL_RenderFillRectF(renderer->gRenderer, &rect);
	}
	else {
		renderer->RenderTexture(entityTexture, position.x, position.y, Alignment::CENTER);
	}
}