#pragma once

#include "entity.h"

class Shard : public Entity
{
public:

	int aliveTicks;

	Shard(vec2_t position, vec2_t direction, vec2_t rotation) : Entity(position, direction, rotation) {};
	Shard(vec2_t position, float speed, float angle) : Entity(position, speed, angle) {};

	void onTick(int deltaTicks) override;
	void onRender() override;
};

