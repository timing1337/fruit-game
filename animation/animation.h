#pragma once

#include "SDL.h"
#include "SDL_timer.h"

#include "game.h"
#include "render.h"

#include <unordered_map>
#include <functional>

using namespace std;

class Renderer;
class GameManager;

class Animation
{
public:
	SDL_TimerID timerId;
	int duration;
	int interval;
	int current = 0;

	bool isReadyForRender = false;

	function<void(Animation* self)> onUpdate;
	function<void(Animation* self)> onComplete;

	Animation(int duration, function<void(Animation* self)> onUpdate, function<void(Animation* self)> onComplete);
	~Animation();

	void Start();
	Uint32 Update(Uint32 interval, void* param);
	void OnComplete();
};

class AnimationManager
{
private:
	static AnimationManager* instancePtr;
public:
	unordered_map<SDL_TimerID, Animation*> animations;
	static AnimationManager* getInstance() {
		return instancePtr;
	}
	void Play(Animation* animation);
};