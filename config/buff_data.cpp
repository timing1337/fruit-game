#include "buff_data.h"

//sort from lowest to highest
std::vector<BuffConfig*> BuffData::configs = {
	new BuffConfig(BuffId::FRUIT_PARTY, 10000, { 255, 255, 255, 255 }, 0.1f),
	new BuffConfig(BuffId::FREEZE, 5000, { 150, 200, 255, 255 }, 0.9f),
};


BuffConfig* BuffData::GetBuffConfigById(BuffId id) {
	for (auto& config : configs) {
		if (config->id == id) {
			return config;
		}
	}
	return nullptr;
}

BuffConfig* BuffData::GetRandomBuffConfig() {
	float randomValue = rand() % 100 / 100.0f;
	for (auto& config : configs) {
		if (randomValue < config->chance) {
			return config;
		}
	}
	return nullptr; // No buff selected
}