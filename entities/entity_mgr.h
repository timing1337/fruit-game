#pragma once

#include "config/fruit_data.h"

#include "entities/particle.h"
#include "entities/entity.h"
#include "utils/math.h"

#include "task/task.h"
#include "task/types/repeated_task.h"

#include <random> 
#include <vector>

using namespace std;

class EntityManager
{
private:
	int entityId = 0;
public:

	bool canSpawnBuff = true;

	static EntityManager* instancePtr;
	vector<Entity*> entities;

	RepeatedTask* spawnTask = nullptr;

	void Initialize();

	static EntityManager* GetInstance() {
		return instancePtr;
	}

	void Heartbeat(int deltaTicks);
	void RandomizeSpawningEntity();
	void CleanUp();

	void spawnEntity(Entity* entity) {
		entities.push_back(entity);
	}

	void spawnParticle(vec2_t position, SDL_Color deathParticleColor);
};