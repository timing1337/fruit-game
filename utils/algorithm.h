#pragma once

#include <vector>

#include "utils/math.h"

//Reference: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
std::vector<SDL_Point> Algorithm_GetPoints(SDL_Point start, SDL_Point end, int thickness = 1);