#pragma once

#include "base_task.h"

class TimerTask : public BaseTask
{
public:
	int counter = 0;
	int duration;

	TimerTask(int duration, std::function<void(TimerTask* self)> onUpdate, std::function<void(TimerTask* self)> onComplete);

	void Update(int deltaTick) override;
	float GetProgress();
};

