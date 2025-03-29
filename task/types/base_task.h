#pragma once

#include <functional>

#include "task/task_state.h"

using namespace std;

class BaseTask
{
public:
	TaskState state = TaskState::WAIT;

	function<void(BaseTask* self)> onUpdate;
	function<void(BaseTask* self)> onComplete;

	BaseTask(function<void(BaseTask* self)> onUpdate, function<void(BaseTask* self)> onComplete);
	~BaseTask();

	void Start();
	virtual void Update(int deltaTick);
	void Kill();
};