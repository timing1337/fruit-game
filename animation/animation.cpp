#include "animation.h"

//Basically a wrapper of SDL_timer

AnimationManager* AnimationManager::instancePtr = new AnimationManager();

Animation::Animation(int duration, function<void(Animation* self)> onUpdate, function<void(Animation* self)> onComplete) {
	this->duration = duration;
	this->onComplete = onComplete;
	this->onUpdate = onUpdate;
}

Uint32 Callback(Uint32 interval, void* param) {
	Animation* animation = static_cast<Animation*>(param);
	return animation->Update(interval, param);
}

Uint32 Animation::Update(Uint32 interval, void* param){
	this->onUpdate(this);
	if (current >= duration) {
		OnComplete();
	}
	else {
		current++;
	}
	isReadyForRender = true;
	return interval;
}

void Animation::OnComplete() {
	SDL_RemoveTimer(timerId);
	AnimationManager::getInstance()->animations.erase(timerId);
	this->onComplete(this);
}

void Animation::Start() {
	timerId = SDL_AddTimer(1, Callback, this);
}

void AnimationManager::Play(Animation* animation) {
	animation->Start();
	this->animations[animation->timerId] = animation;
}