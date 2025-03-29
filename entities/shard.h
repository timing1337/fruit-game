#pragma once

#include "entity.h"

class Shard : public Entity
{
public:
	void onRender();
	void onDespawn(EntityDeathType type);
	void onHit();
	bool IsCollidingWithPoint(int x, int y);
};

