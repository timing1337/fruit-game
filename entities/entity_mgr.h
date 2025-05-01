#pragma once

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
	static EntityManager* instancePtr;
	vector<Entity*> entities;

	mt19937_64 entity_random_mt;

	//entity random gen
	uniform_real_distribution<float> entityDistribution;
	uniform_int_distribution<int> spawnDirection;
	uniform_int_distribution<int> xBound;
	uniform_int_distribution<int> yBound;

	//particle random
	uniform_real_distribution<float> particleRandomPosition;
	uniform_real_distribution<float> particleRandomAngle;
	uniform_real_distribution<float> particleRandonSpeed;

	RepeatedTask* spawnTask = nullptr;

	void Initialize();

	static EntityManager* GetInstance() {
		return instancePtr;
	}

	void Heartbeat(int deltaTicks);
	void RandomizeSpawningEntity();
	void CleanUp();

	template <typename T>
	void spawnEntity(T* entity) {
		entities.push_back(entity);
	}

	void spawnParticle(vec2_t position);
};