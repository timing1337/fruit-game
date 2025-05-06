#include "algorithm.h"

vector<SDL_Point> Algorithm_GetPoints(SDL_Point start, SDL_Point end, int thickness) {
	vector<SDL_Point> points;

	int dx = abs(end.x - start.x);
	int sx = start.x < end.x ? 1 : -1;
	int dy = -abs(end.y - start.y);
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;

	while (true)
	{
		for (int y = -thickness; y <= thickness; ++y) {
			for (int x = -thickness; x <= thickness; ++x) {
				int px = start.x + x;
				int py = start.y + y;
				if (px < 0 || py < 0) continue;
				points.push_back(SDL_Point{ px, py });
			}
		}

		int e2 = 2 * error;
		if (e2 >= dy) {
			if (start.x == end.x) break;
			error += dy;
			start.x += sx;
		}
		else if (e2 <= dx) {
			if (start.y == end.y) break;
			error += dx;
			start.y += sy;
		}
	}

	return points;
}