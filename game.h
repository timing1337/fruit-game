#pragma once

#include "SDL.h"

#include "entities/entity_mgr.h"
#include "entities/enemy.h"
#include "mouse_path.h"
#include "render.h"
#include "game_state.h"

class GameManager
{
public:
	static GameManager* instancePtr;

	//Game state
	bool running = true;
	GameState state = GameState::RUNNING;
	int deltaTime = 0;

	//Game Data
	int score = 0;
	int remainingLives = 3;
	int lastUpdatedTicks = 0;

	//Mouse path recording
	MousePathRecord* mousePathRecord = new MousePathRecord();

	void Heartbeat();

	void FireStateChange(GameState state);

	void OnMouseClick(SDL_MouseButtonEvent& e);
	void OnMouseRelease(SDL_MouseButtonEvent& e);
	void OnMouseMove(SDL_MouseButtonEvent& e);

	void OnWaiting();
	void OnStarting();
	void OnRunning();
	void OnPostgame();

	void AddScore(int score);
	void SetScore(int score);

	static GameManager* getInstance() {
		return instancePtr;
	}
};

