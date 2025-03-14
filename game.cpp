#include "game.h"

GameManager::GameManager() {

}

void GameManager::Heartbeat() {
	
}

void GameManager::FireStateChange(GameState state) {
	this->state = state;
	switch (state) {
	case GameState::WAITING:
		OnWaiting();
		break;
	case GameState::STARTING:
		OnStarting();
		break;
	case GameState::RUNNING:
		OnRunning();
		break;
	case GameState::POSTGAME:
		OnPostgame();
		break;
	}
}

void GameManager::OnWaiting() {
}

void GameManager::OnStarting() {
}

void GameManager::OnRunning() {
}

void GameManager::OnPostgame() {
}

GameManager::~GameManager() {

}