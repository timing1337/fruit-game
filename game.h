#pragma once

#include "SDL.h";

enum GameState {
	WAITING,
	STARTING,
	RUNNING,
	POSTGAME
};

class GameManager
{
public:
	bool running = true;

	GameState state = GameState::WAITING;

	GameManager();
	~GameManager();

	void Heartbeat();

	void FireStateChange(GameState state);

	void OnWaiting();
	void OnStarting();
	void OnRunning();
	void OnPostgame();
};

