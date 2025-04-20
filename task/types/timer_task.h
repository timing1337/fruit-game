#pragma once

#include "base_task.h"

class TimerTask : public BaseTask
{
public:
	int counter = 0;
	int duration;

	TimerTask(int duration, function<void(TimerTask* self)> onUpdate, function<void(TimerTask* self)> onComplete);

	void Update(int deltaTick) override;
	float GetProgress();
};

