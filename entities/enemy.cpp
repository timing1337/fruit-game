#include "enemy.h"

void Enemy::onHit() {
	this->despawn(EntityDeathType::PLAYER_INTERACTION);
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
		GameManager::getInstance()->UpdateScore(this->scoreBase);
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

void Enemy::onRender() {
	Renderer* renderer = Renderer::getInstance();
	SDL_SetRenderDrawColor(renderer->gRenderer, 255, 255, 255, 255);

	SDL_FRect rect = { position.x - 15, position.y - 15, 30, 30 };
	SDL_RenderFillRectF(renderer->gRenderer, &rect);
}

bool Enemy::IsCollidingWithPoint(int x, int y){
	return isPointInRect({ x, y }, { (int)position.x - 15, (int)position.y - 15 }, { 30, 30 });
}