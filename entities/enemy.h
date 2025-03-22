#pragma once

#include "entities/entity.h"

class Entity;

class Enemy: public Entity
{
public:
	void onRender();
	void onDespawn(EntityDeathType type);
	void onHit();
};