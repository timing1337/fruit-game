#pragma once

#include "SDL.h"
#include "render.h"
#include "blur.h"
#include "utils/math.h"

#include <vector>

using namespace std;

SDL_Surface* SDL_Downsampling(SDL_Surface* surface, float size);
SDL_Surface* SDL_BoxBlur3by3(SDL_Surface* surface);