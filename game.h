#pragma once

#include "SDL.h";

enum GameState {
	WAITING,
	PREPARING,
	STARTING,
	RUNNING,
	POSTGAME
};

class GameManager
{
public:
	static GameManager* instancePtr;

	bool running = true;
	GameState state = GameState::WAITING;

	int score = 0;
	int remainingLives = 3;

	int ticks = 0;

	void Heartbeat();

	void FireStateChange(GameState state);

	void OnWaiting();
	void OnStarting();
	void OnRunning();
	void OnPostgame();
	static GameManager* getInstance() {
		return instancePtr;
	}
};

