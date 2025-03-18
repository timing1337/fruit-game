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
			this->OnMouseMove(e.button);
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

	this->mousePathRecord->isRecording = true;

	this->mousePathRecord->paths.clear();
	this->mousePathRecord->AddPoint(SDL_Point{ e.x, e.y });
}

void GameManager::OnMouseRelease(SDL_MouseButtonEvent& e) {
	if (e.button != SDL_BUTTON_LEFT) {
		return;
	}

	if (this->state != GameState::RUNNING) {
		return;
	}

	if (!this->mousePathRecord->isRecording) {
		return;
	}

	this->mousePathRecord->isRecording = false;
	this->mousePathRecord->AddPoint(SDL_Point{ e.x, e.y });

	if (this->mousePathRecord->paths.size() < 5) {
		return;
	}

	MousePathRecord record = *this->mousePathRecord;

	Renderer::getInstance()->OnMousePathRecorded(record);
}

void GameManager::OnMouseMove(SDL_MouseButtonEvent& e) {
	if (this->state != GameState::RUNNING) {
		return;
	}

	if (!this->mousePathRecord->isRecording) {
		return;
	}

	SDL_Point currentPoint = SDL_Point{ e.x, e.y };
	this->mousePathRecord->AddPoint(SDL_Point{ currentPoint.x, currentPoint.y });
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