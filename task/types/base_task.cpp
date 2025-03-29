#include "base_task.h"

BaseTask::BaseTask(function<void(BaseTask* self)> onUpdate, function<void(BaseTask* self)> onComplete) {
	this->onUpdate = onUpdate;
	this->onComplete = onComplete;
}

BaseTask::~BaseTask() {
	if (this->state != TaskState::KILLED) {
		//task got disposed before it kills
		this->Kill();
	}
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