#include "repeated_task.h"
#include <iostream>

RepeatedTask::RepeatedTask(int interval, std::function<void(RepeatedTask* self)> onUpdate, std::function<void(RepeatedTask* self)> onComplete) :
	BaseTask(
        [onUpdate](BaseTask* self) { onUpdate(static_cast<RepeatedTask*>(self)); }, 
        [onComplete](BaseTask* self) { onComplete(static_cast<RepeatedTask*>(self)); }
    ){
	this->interval = interval;
}

void RepeatedTask::Update(int deltaTime) {
	if (counter >= interval) {
		counter = 0;
		onUpdate(this);
	}
	else {
		counter += deltaTime;
	}
}