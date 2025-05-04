#include "fruit_data.h"

vector<FruitConfig> FruitData::configs = {
	FruitConfig({"pineapple", "fruits/pineapple.png", 2, {255, 185, 91, 255}}),
	FruitConfig({"watermelon", "fruits/watermelon.png", 4, {255, 91, 91, 255}}),
	FruitConfig({"banana", "fruits/banana.png", 2, {255, 255, 91, 255}}),
	FruitConfig({"apple", "fruits/apple.png", 1, {255, 50, 50, 255}}),
};

FruitConfig* FruitData::GetFruitConfigByName(const char* name) {
	for (auto& config : configs) {
		if (strcmp(config.name, name) == 0) {
			return &config;
		}
	}
	return nullptr;
}

FruitConfig FruitData::GetRandomFruitConfig() {
	int randomIndex = rand() % configs.size();
	return configs[randomIndex];
}