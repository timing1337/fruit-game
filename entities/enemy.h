#pragma once

#include "game.h"
#include "entities/shard.h"
#include "entities/entity.h"
#include "utils/math.h"

class Enemy: public Entity
{
public:
	int scoreBase = 1;

	Enemy(vec2_t position, vec2_t direction, vec2_t rotation) : Entity(position, direction, rotation) {};
	Enemy(vec2_t position, float speed, float angle) : Entity(position, speed, angle) {};

	void onTick(int deltaTicks) override;
	void onRender() override;
	void onDespawn(EntityDeathType type) override;
	void onHit() override;
};