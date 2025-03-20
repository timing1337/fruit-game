#pragma once

#
#include "utils/math.h"

#include <vector>

struct MousePath {
	SDL_Point point;
	int longevity;
};

struct MousePathRecord {
	bool isRecording = false;
	std::vector<MousePath> paths;

	MousePathRecord() {
		paths = std::vector<MousePath>();
	};

	void AddPoint(SDL_Point point) {
		MousePath path;
		path.point = point;
		path.longevity = 1000; // half second for staying and half second for fading
		paths.push_back(path);
	}
};
