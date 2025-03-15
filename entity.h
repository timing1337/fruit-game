#pragma once

#include "game.h"
#include "textures.h"
#include "utils/math.h"

class Entity
{
public:
	GameTexture* entityTexture;
	vec2_t position;
	vec2_t direction;

	int lifeTicks = 0;
	bool alive = true;

	void onTick();
	void onDeath();
};

class EntityManager
{
private:
	int entityId = 0;
public:
	static EntityManager* instancePtr;
	std::vector<Entity> entities;
	static EntityManager* getInstance() {
		return instancePtr;
	}

	void spawnEntity(vec2_t position, vec2_t direction);
	void spawnEntity(vec2_t position, float degree, float speed);
	void Heartbeat();
};