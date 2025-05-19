#include "task.h"

TaskManager* TaskManager::instancePtr = new TaskManager();

RepeatedTask* TaskManager::RunRepeatedTask(int interval, std::function<void(RepeatedTask* self)> onUpdate, std::function<void(RepeatedTask* self)> onComplete) {
	RepeatedTask* task = new RepeatedTask(interval, onUpdate, onComplete);
	animations.push_back(task);
	task->Start();
	return task;
}

TimerTask* TaskManager::RunTimerTask(int duration, std::function<void(TimerTask* self)> onUpdate, std::function<void(TimerTask* self)> onComplete) {
	TimerTask* task = new TimerTask(duration, onUpdate, onComplete);
	animations.push_back(task);
	task->Start();
	return task;
}

void TaskManager::Heartbeat(int deltaTick) {
	for (int i = 0; i < animations.size(); i++) {
		BaseTask* task = animations[i];
		if (task->state == TaskState::RUN) {
			task->Update(deltaTick);
		}
		if (task->state == TaskState::KILLED) {
			animations.erase(animations.begin() + i);
			delete task;
		}
	}
}