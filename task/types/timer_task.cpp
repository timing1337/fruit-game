#include "timer_task.h"


TimerTask::TimerTask(int duration, std::function<void(TimerTask* self)> onUpdate, std::function<void(TimerTask* self)> onComplete) :
	BaseTask(
		[onUpdate](BaseTask* self) { onUpdate(static_cast<TimerTask*>(self)); },
		[onComplete](BaseTask* self) { onComplete(static_cast<TimerTask*>(self)); }
	) {
	this->duration = duration;
}


void TimerTask::Update(int deltaTime) {
	this->onUpdate(this);

	this->counter += deltaTime;

	if (counter >= duration) {
		this->onComplete(this);
		this->Kill();
	}
}

float TimerTask::GetProgress() {
	return (float)this->counter / this->duration;
}