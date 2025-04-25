#include "enemy.h"

void Enemy::onHit() {
	this->despawn(EntityDeathType::PLAYER_INTERACTION);
}

void Enemy::onTick(int deltaTicks) {
	Entity::onTick(deltaTicks);
}

void Enemy::onDespawn(EntityDeathType type) {

	GameManager* game_mgr = GameManager::getInstance();
	Renderer* renderer = Renderer::getInstance();

	switch (type) {
	case EntityDeathType::OUT_OF_BOUND:
		game_mgr->remainingLives--;

		game_mgr->SetCombo(0);

		if (game_mgr->remainingLives == 0) {
			game_mgr->FireStateChange(GameState::POSTGAME);
			renderer->PlayFadeTransition([this, game_mgr](BaseTask* self) {
				game_mgr->FireStateChange(GameState::ENDGAME);
			}, [this](BaseTask* self) {});
		}
		else {
			TaskManager::getInstance()->RunTimerTask(ENTITY_DEATH_IMPACT,
				[renderer](TimerTask* self) {
					int calculatedOpacity = max(100 - ((int)(self->GetProgress() * 100)), 0);
					renderer->SetBackgroundColor(255, 101, 88, calculatedOpacity);
				}, [](BaseTask* self) {});
		}
		break;
		//Reduce player life
	case EntityDeathType::PLAYER_INTERACTION:
		game_mgr->AddScore(this->scoreBase);
		game_mgr->AddCombo(1);
		TaskManager::getInstance()->RunTimerTask(ENTITY_KILL_IMPACT,
			[renderer](TimerTask* self) {
				int calculatedOpacity = max(100 - ((int)(self->GetProgress() * 100)), 0);
				renderer->SetBackgroundColor(127, 250, 160, calculatedOpacity);
			}, [](BaseTask* self) {});

		EntityManager::getInstance()->spawnParticle(this->position);
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
		SDL_Rect rect = { position.x - hitbox.x / 2, position.y - hitbox.y / 2, hitbox.x, hitbox.y };
		SDL_RenderFillRect(renderer->gRenderer, &rect);
	}
	else {
		renderer->RenderTexture(entityTexture, position.x, position.y, Alignment::CENTER);
	}
}