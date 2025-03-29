#pragma once

#include "game.h"
#include "entities/entity.h"
#include "utils/math.h"

class Enemy: public Entity
{
public:
	int scoreBase = 1;

	void onRender();
	void onDespawn(EntityDeathType type);
	void onHit();
	bool IsCollidingWithPoint(int x, int y);
};