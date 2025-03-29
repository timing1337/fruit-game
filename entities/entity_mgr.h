#pragma once

#include "entities/entity.h"
#include "entities/enemy.h"
#include "utils/math.h"


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

	static EntityManager* getInstance() {
		return instancePtr;
	}

	void Heartbeat(int deltaTicks);

	template <typename T>
	void spawnEntity(vec2_t position, vec2_t direction, vec2_t rotation = vec2_t(0, 0)) {
		T* entity = new T();
		entity->position = position;
		entity->direction = direction;
		entity->rotation = rotation;

		entities.push_back(entity);
	}

	template <typename T>
	void spawnEntity(vec2_t position, float speed, float angle, vec2_t rotation = vec2_t(0, 0)) {
		float radian = deg2rad(angle);
		EntityManager::spawnEntity<T>(position, vec2_t(speed * cos(radian), speed * sin(radian)), rotation);
	}
};