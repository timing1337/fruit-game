#pragma once

#
#include "utils/math.h"

#include <vector>

struct MousePathRecord {
	std::vector<vec2_t> paths;

	MousePathRecord() {
		paths = std::vector<vec2_t>();
	};
};
