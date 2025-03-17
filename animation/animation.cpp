#include "animation.h"

//Basically a wrapper of SDL_timer

AnimationManager* AnimationManager::instancePtr = new AnimationManager();

Animation::Animation(int duration, function<void(Animation* self)> onUpdate, function<void(Animation* self)> onComplete) {
	this->duration = duration;
	this->onComplete = onComplete;
	this->onUpdate = onUpdate;
}

Animation::~Animation() {

}

void Animation::Update() {
	this->onUpdate(this);
	current += GameManager::getInstance()->deltaTime;
	if (current >= duration) {
		OnComplete();
	}
}


void Animation::Start() {
	this->state = AnimationState::RUN;
}

void Animation::OnComplete() {
	this->onComplete(this);
	this->state = AnimationState::KILLED;
}

void AnimationManager::Play(int duration, function<void(Animation* self)> onUpdate, function<void(Animation* self)> onComplete) {
	Animation animation = Animation(duration, onUpdate, onComplete);
	animation.Start();
	this->animations.push_back(animation);
}

void AnimationManager::Heartbeat() {
	if (this->animations.size() == 0) return;
	for (int i = 0; i < this->animations.size(); i++) {
		Animation* animation = &this->animations[i];
		switch (animation->state)
		{
		case AnimationState::RUN:
			animation->Update();
			break;
		case AnimationState::KILLED:
			this->animations.erase(this->animations.begin() + i);
			break;
		default:
			break;
		}
	}
}