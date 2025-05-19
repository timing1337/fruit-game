#pragma once

#include <functional>

#include "task/task_state.h"



class BaseTask
{
public:
	TaskState state = TaskState::WAIT;

	std::function<void(BaseTask* self)> onUpdate;
	std::function<void(BaseTask* self)> onComplete;

	BaseTask(std::function<void(BaseTask* self)> onUpdate, std::function<void(BaseTask* self)> onComplete);
	virtual ~BaseTask() = default;

	void Start();
	virtual void Update(int deltaTick);
	void Kill();
};