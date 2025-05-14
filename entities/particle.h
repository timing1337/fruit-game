#pragma once

#include "game/game.h"
#include "entity.h"

class Particle : public Entity
{
public:

	SDL_Color color = { 255, 255, 255, 255 };

	int aliveTicks = 0;
	int maxAliveTicks;
	Particle(vec2_t position, vec2_t direction, vec2_t rotation, int maxAliveTicks) : Entity(position, direction), maxAliveTicks(maxAliveTicks) {};
	Particle(vec2_t position, float speed, float angle, int maxAliveTicks) : Entity(position, speed, angle), maxAliveTicks(maxAliveTicks) {};

	void onTick(int deltaTicks) override;
	void onRender() override;
	void onDespawn(EntityDeathType type) override;
	void onHit() override;
};

