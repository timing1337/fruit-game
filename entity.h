#pragma once

#include "game.h"
#include "textures.h"
#include "utils/algorithm.h"
#include "utils/math.h"

class Entity
{
public:
	GameTexture* entityTexture;
	vec2_t position;
	vec2_t direction;
	vec2_t rotation;

	int collidingWithMouseTicks = 0;

	int lifeTicks = 0;
	bool alive = true;

	void onTick();
	void onDeath();
	void onRender();
	void onHit();

	void despawn();

	bool IsCollidingWithPoint(int x, int y);
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

	void spawnEntity(vec2_t position, vec2_t direction, vec2_t rotation = vec2_t(0, 0));
	void spawnEntity(vec2_t position, float velocityAngle, float speed, vec2_t rotation = vec2_t(0, 0));
	void Heartbeat();
};