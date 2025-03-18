#pragma once

#include "SDL.h"

void SDL_SurfaceDrawLines(SDL_Surface* surface, const SDL_Point* points, const int count, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a, const int thickness = 1);
void SDL_SurfaceDrawLine(SDL_Surface* surface, SDL_Point start, SDL_Point end, const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a, const int thickness = 1);