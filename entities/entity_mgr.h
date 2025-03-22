#pragma once

#include "entities/entity.h"

#include <vector>

using namespace std;

class EntityManager
{
private:
	int entityId = 0;
public:
	static EntityManager* instancePtr;
	vector<Entity> entities;
	static EntityManager* getInstance() {
		return instancePtr;
	}

	void Heartbeat();
};
