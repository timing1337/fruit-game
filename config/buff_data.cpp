#include "buff_data.h"

vector<BuffConfig*> BuffData::configs = {
	new BuffConfig(BuffId::FREEZE, 3000, { 204, 255, 255, 255 }),
	new BuffConfig(BuffId::DOUBLE_SCORE, 5000, { 255, 255, 255, 255 }),
	new BuffConfig(BuffId::FRUIT_PARTY, 4000, { 255, 255, 255, 255 }),
};