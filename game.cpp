#include "game.h"

GameManager* GameManager::instancePtr = new GameManager();

void GameManager::Heartbeat() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) {
		case SDL_QUIT:
			this->running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			Renderer::getInstance()->OnMouseClick(e.button);
			this->OnMouseClick(e.button);
			break;
		case SDL_MOUSEBUTTONUP:
			this->OnMouseRelease(e.button);
			break;
		case SDL_MOUSEMOTION:
			this->OnMouseMove(e.motion);
			break;
		}
	}
}

void GameManager::OnMouseClick(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}

	if (this->state != GameState::RUNNING) {
		return;
	}

	this->isRecording = true;

	this->currentRecord->paths.clear();
	this->currentRecord->paths.push_back({ e.x, e.y });
}

void GameManager::OnMouseRelease(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}

	if (this->state != GameState::RUNNING) {
		return;
	}

	if (!this->isRecording) {
		return;
	}

	this->isRecording = false;
	this->currentRecord->paths.push_back(SDL_Point{ e.x, e.y });

	if (this->currentRecord->paths.size() < 5) {
		return;
	}

	Renderer::getInstance()->OnMousePathRecorded(*(MousePathRecord*)this->currentRecord);
}

void GameManager::OnMouseMove(SDL_MouseMotionEvent& e) {
	if (this->state != GameState::RUNNING) {
		return;
	}

	if (!this->isRecording) {
		return;
	}

	this->currentRecord->paths.push_back({ e.x, e.y });

	if (this->currentRecord->paths.size() > 200) {
		//Force end
		this->isRecording = false;
		Renderer::getInstance()->OnMousePathRecorded(*(MousePathRecord*)this->currentRecord);
	}
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
	this->remainingLives = 3;
	this->score = 0;
}

void GameManager::OnRunning() {
}

void GameManager::OnPostgame() {
}