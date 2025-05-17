#include "buff_data.h"

vector<BuffConfig*> BuffData::configs = {
	new BuffConfig(BuffId::FREEZE, 3000, { 0, 100, 255, 255 }),
	new BuffConfig(BuffId::DOUBLE_SCORE, 5000, { 255, 255, 255, 255 }),
	new BuffConfig(BuffId::FRUIT_PARTY, 4000, { 255, 255, 255, 255 }),
};


BuffConfig* BuffData::GetBuffConfigById(BuffId id) {
	for (auto& config : configs) {
		if (config->id == id) {
			return config;
		}
	}
	return nullptr;
}