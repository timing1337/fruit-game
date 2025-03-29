#include "enemy.h"

void Enemy::onHit() {
	this->despawn(EntityDeathType::PLAYER_INTERACTION);
}

void Enemy::onDespawn(EntityDeathType type) {
	switch (type) {
	case EntityDeathType::OUT_OF_BOUND:
		GameManager::getInstance()->remainingLives--;
		if (GameManager::getInstance()->remainingLives <= 0) {
			GameManager::getInstance()->FireStateChange(GameState::POSTGAME);
		}
		break;
		//Reduce player life
	case EntityDeathType::PLAYER_INTERACTION:
		MainScene::killEffectFade = 300;
		GameManager::getInstance()->UpdateScore(1);
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