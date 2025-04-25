#pragma once

#include "constant.h"
#include "utils/math.h"

#include <vector>

struct MousePath {
	SDL_Point point;
	int longevity;
};

struct MousePathRecord {
	std::vector<MousePath> paths;
	float distance = 0;

	MousePathRecord() {
		paths = std::vector<MousePath>();
	};

	void AddPoint(SDL_Point point) {
		MousePath path;
		path.point = point;
		path.longevity = POINT_LONGEVITY;
		paths.push_back(path);
	}

	void RecalculateDistance() {
		this->distance = 0;
		if (this->paths.size() < 2) {
			return;
		}
		for (int i = 0; i < this->paths.size() - 1; i++) {
			MousePath& path = this->paths[i];
			MousePath& lastPath = this->paths[i + 1];
			this->distance += (float)sqrt(pow(path.point.x - lastPath.point.x, 2) + pow(path.point.y - lastPath.point.y, 2));

		}
	}
};
