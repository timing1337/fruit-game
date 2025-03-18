#include "surface_draw.h"

void SDL_SurfaceDrawLines(SDL_Surface* surface, const SDL_Point* points, const int count, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a, const int thickness){
	for (int i = 0; i < count - 1; i++) {
		SDL_Point start = points[i];
		SDL_Point end = points[i + 1];
		SDL_SurfaceDrawLine(surface, start, end, r, g, b, a, thickness);
	}
}

void SDL_SurfaceDrawLine(SDL_Surface* surface, SDL_Point start, SDL_Point end, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a, const int thickness){
	SDL_LockSurface(surface);

	int color = SDL_MapRGBA(surface->format, r, g, b, a);
	int pitch = surface->pitch / sizeof(unsigned int);

	Uint32* pixels = (Uint32*)surface->pixels;

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
				if (px >= 0 && px < surface->w && py >= 0 && py < surface->h) {
					pixels[py * pitch + px] = color;
				}
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

	SDL_UnlockSurface(surface);
}