#pragma once

#
#include "utils/math.h"

#include <vector>

struct MousePathRecord {
	std::vector<SDL_Point> paths;

	MousePathRecord() {
		paths = std::vector<SDL_Point>();
	};
};
