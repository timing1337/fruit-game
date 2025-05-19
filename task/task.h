#pragma once

#include "SDL.h"

#include "task/types/base_task.h"
#include "task/types/timer_task.h"
#include "task/types/repeated_task.h"

#include <functional>
#include <vector>



class TaskManager
{
private:
	static TaskManager* instancePtr;
public:
	int timerId = 0;
	int lastUpdatedTick = 0;
	std::vector<BaseTask*> animations;
	static TaskManager* GetInstance() {
		return instancePtr;
	}

	RepeatedTask* RunRepeatedTask(int interval, std::function<void(RepeatedTask* self)> onUpdate, std::function<void(RepeatedTask* self)> onComplete);
	TimerTask* RunTimerTask(int duration, std::function<void(TimerTask* self)> onUpdate, std::function<void(TimerTask* self)> onComplete);

	void Heartbeat(int deltaTick);
};