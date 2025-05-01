#pragma once

#include "SDL.h"

#include "task/types/base_task.h"
#include "task/types/timer_task.h"
#include "task/types/repeated_task.h"

#include <functional>
#include <vector>

using namespace std;

class TaskManager
{
private:
	static TaskManager* instancePtr;
public:
	int timerId = 0;
	int lastUpdatedTick = 0;
	vector<BaseTask*> animations;
	static TaskManager* GetInstance() {
		return instancePtr;
	}

	RepeatedTask* RunRepeatedTask(int interval, function<void(RepeatedTask* self)> onUpdate, function<void(RepeatedTask* self)> onComplete);
	TimerTask* RunTimerTask(int duration, function<void(TimerTask* self)> onUpdate, function<void(TimerTask* self)> onComplete);

	void Heartbeat(int deltaTick);
};