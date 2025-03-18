#pragma once

#include "SDL.h"

#include "utils/math.h"

#include <vector>

using namespace std;

void SDL_PostGaussianBlur(SDL_Surface* surface, float radius = 0);
void SDL_PostBrighten(SDL_Surface* surface);