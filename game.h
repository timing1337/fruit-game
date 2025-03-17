#pragma once

#include "render.h"

#include "mouse_path.h"

class Renderer;

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

	//Game state
	bool running = true;
	GameState state = GameState::WAITING;
	float deltaTime = 0;

	//Game Data
	int score = 0;
	int remainingLives = 3;
	int lastUpdatedTicks = 0;

	//Mouse path recording
	bool isRecording = false;
	MousePathRecord* currentRecord = new MousePathRecord();

	void Heartbeat();

	void FireStateChange(GameState state);

	void OnMouseClick(SDL_MouseButtonEvent& e);
	void OnMouseRelease(SDL_MouseButtonEvent& e);
	void OnMouseMove(SDL_MouseMotionEvent& e);

	void OnWaiting();
	void OnStarting();
	void OnRunning();
	void OnPostgame();

	static GameManager* getInstance() {
		return instancePtr;
	}
};

