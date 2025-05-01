#pragma once

#include "game/game.h"
#include "entity.h"

class Particle : public Entity
{
public:

	int aliveTicks = 0;

	Particle(vec2_t position, vec2_t direction, vec2_t rotation) : Entity(position, direction) {};
	Particle(vec2_t position, float speed, float angle) : Entity(position, speed, angle) {};

	void onTick(int deltaTicks) override;
	void onRender() override;
	void onDespawn(EntityDeathType type) override;
	void onHit() override;
};

