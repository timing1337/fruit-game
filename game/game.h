#pragma once

#include "SDL.h"

#include "config/buff_data.h"
#include "entities/entity_mgr.h"
#include "entities/enemy.h"
#include "scene/scene_manager.h"
#include "mouse_record/mouse_path.h"
#include "game_state.h"
#include "game_data.h"
#include "utils/algorithm.h"

class GameManager
{
public:
	static GameManager* instancePtr;
	static GameManager* GetInstance() {
		return instancePtr;
	}

	//Game state
	bool running = true;
	GameState state = GameState::WAITING;
	int deltaTime = 0;

	//Game Data
	GameData* gameData = nullptr;

	//Timing
	int lastUpdatedTicks = 0;

	//Runtime game data
	int remainingLives = 3;

	int score = 0;

	int currentCombo = 0;
	int highestComboRecorded = 0;
	int comboExpirationTick = 0;

	int slicedFruit = 0;

	// Buff
	BuffId activeBuff = BUFF_NONE;
	TimerTask* buffTask = nullptr;
	int buffChannelId = -1;

	//Mouse path recording
	MousePathRecord* mousePathRecord = nullptr;
	std::vector<MousePathRecord*> mousePathRecordsLeftover;

	void Initialize();

	void Heartbeat(int deltaTicks);

	void FireStateChange(GameState state);

	void OnMouseClick(SDL_MouseButtonEvent& e);
	void OnMouseRelease(SDL_MouseButtonEvent& e);
	void OnMouseMove(SDL_MouseButtonEvent& e);

	void ResetRuntimeGameData();

	void AddScore(int score);
	void SetScore(int score);

	void SetCombo(int combo);
	void AddCombo(int combo);

	void SetRemainingLives(int lives);

	void TriggerBuff(BuffConfig* buff);
};

