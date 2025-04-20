#pragma once

#include "entities/particle.h"
#include "entities/entity.h"
#include "utils/math.h"

#include "task/task.h"
#include "task/types/repeated_task.h"

#include <chrono>
#include <ctime>  
#include <iostream> 
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
	mt19937 mt;
	uniform_real_distribution<float> distribution;
	uniform_int_distribution<int> spawnDirection;
	uniform_int_distribution<int> xBound;
	uniform_int_distribution<int> yBound;

	RepeatedTask* spawnTask = nullptr;

	void Initialize();

	static EntityManager* getInstance() {
		return instancePtr;
	}

	void Heartbeat(int deltaTicks);
	void CleanUp();
	void spawnTaskCallback(RepeatedTask* self);

	template <typename T>
	void spawnEntity(T* entity) {
		entities.push_back(entity);
	}

	void spawnParticle(vec2_t position);
};