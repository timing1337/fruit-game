#pragma once

#
#include "utils/math.h"

#include <vector>

struct MousePathRecord {
	bool isRecording = false;
	std::vector<SDL_Point> paths;

	MousePathRecord() {
		paths = std::vector<SDL_Point>();
	};

	void AddPoint(SDL_Point point, int sizeIncrement = 1) {
		paths.push_back(SDL_Point{ point.x, point.y});
	}
};
