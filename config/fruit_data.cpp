#include "fruit_data.h"

std::vector<FruitConfig> FruitData::configs = {
	FruitConfig({"pineapple", "fruits/pineapple.png", "fruit/pineapple.wav",2, 6, {255, 185, 91, 255}}),
	FruitConfig({"watermelon", "fruits/watermelon.png", "fruit/watermelon.wav", 4, 10, {255, 91, 91, 255}}),
	FruitConfig({"banana", "fruits/banana.png", "fruit/banana.wav", 2, 3, {255, 255, 91, 255}}),
	FruitConfig({"apple", "fruits/apple.png", "fruit/apple.wav", 1, 3, {255, 50, 50, 255}}),
};

FruitConfig* FruitData::GetFruitConfigByName(const char* name) {
	for (auto& config : configs) {
		if (strcmp(config.name, name) == 0) {
			return &config;
		}
	}
	return nullptr;
}