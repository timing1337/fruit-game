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