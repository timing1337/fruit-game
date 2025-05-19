#include "base_task.h"

BaseTask::BaseTask(std::function<void(BaseTask* self)> onUpdate, std::function<void(BaseTask* self)> onComplete) {
	this->onUpdate = onUpdate;
	this->onComplete = onComplete;
}

void BaseTask::Update(int deltaTick){
}

void BaseTask::Start() {
	this->state = TaskState::RUN;
}

void BaseTask::Kill() {
	this->onComplete(this);
	this->state = TaskState::KILLED;
}