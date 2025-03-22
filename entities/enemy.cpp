#include "enemy.h"

void Enemy::onHit() {
	this->despawn(EntityDeathType::PLAYER_INTERACTION);
}

void Enemy::onDespawn(EntityDeathType type) {
	switch (type) {
	case EntityDeathType::OUT_OF_BOUND:
		//Reduce player life
		break;
	case EntityDeathType::PLAYER_INTERACTION:
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