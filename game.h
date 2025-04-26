#pragma once

#include "SDL.h"

#include "entities/entity_mgr.h"
#include "entities/enemy.h"
#include "mouse_path.h"
#include "game_state.h"

#include <ctime>  
#include <random> 
#include <game_data.h>

class GameManager
{
public:
	static GameManager* instancePtr;

	//Game state
	bool running = true;
	GameState state = GameState::RUNNING;
	int deltaTime = 0;

	mt19937 mt;
	uniform_real_distribution<float> comboDistribution;

	//Game Data
	GameData* gameData;
	int score = 0;
	int remainingLives = 3;
	int lastUpdatedTicks = 0;
	int currentCombo = 0;
	int comboExpirationTick = 0;

	//fast hack
	bool isOnStateChange = false;

	//Mouse path recording
	MousePathRecord* mousePathRecord = new MousePathRecord();
	vector<MousePathRecord*> mousePathRecordsLeftover;

	void Initialize();

	void Heartbeat(int deltaTicks);

	void FireStateChange(GameState state);

	void OnMouseClick(SDL_MouseButtonEvent& e);
	void OnMouseRelease(SDL_MouseButtonEvent& e);
	void OnMouseMove(SDL_MouseButtonEvent& e);

	void OnWaiting();
	void OnStarting();
	void OnRunning();
	void OnPostgame();

	void ResetGameData();

	void AddScore(int score);
	void SetScore(int score);

	void SetCombo(int combo);
	void AddCombo(int combo);
	void ResetCombo();

	static GameManager* getInstance() {
		return instancePtr;
	}
};

