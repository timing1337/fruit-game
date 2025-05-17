#include "buff_data.h"

vector<BuffConfig*> BuffData::configs = {
	new BuffConfig(BuffId::FREEZE, 5000, { 150, 200, 255, 255 }),
	new BuffConfig(BuffId::DOUBLE_SCORE, 5000, { 255, 255, 255, 255 }),
	new BuffConfig(BuffId::FRUIT_PARTY, 10000, { 255, 255, 255, 255 }),
};


BuffConfig* BuffData::GetBuffConfigById(BuffId id) {
	for (auto& config : configs) {
		if (config->id == id) {
			return config;
		}
	}
	return nullptr;
}