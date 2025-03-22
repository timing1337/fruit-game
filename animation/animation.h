#pragma once

#include "SDL.h"

#include "game.h"
#include "render.h"

#include <functional>
#include <vector>

using namespace std;

enum AnimationState {
	WAIT,
	RUN,
	KILLED
};

class Animation
{
public:
	int timerId;
	int duration;
	int current = 0;

	AnimationState state = AnimationState::WAIT;

	function<void(Animation* self)> onUpdate;
	function<void(Animation* self)> onComplete;

	Animation(int duration, function<void(Animation* self)> onUpdate, function<void(Animation* self)> onComplete);
	~Animation();

	void Start();
	void Update();
	void Kill();
};

class AnimationManager
{
private:
	static AnimationManager* instancePtr;
public:
	int timerId = 0;
	int lastUpdatedTick = 0;
	vector<Animation> animations;
	static AnimationManager* getInstance() {
		return instancePtr;
	}
	void Play(int duration, function<void(Animation* self)> onUpdate, function<void(Animation* self)> onComplete);
	void Heartbeat();
};