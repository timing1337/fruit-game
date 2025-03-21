#include "surface_draw.h"
#include "utils/algorithm.h"

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

	vector<SDL_Point> points = getLinePoints(start, end, thickness);

	for (auto& point : points) {
		pixels[point.y * pitch + point.x] = color;
	}

	SDL_UnlockSurface(surface);
}