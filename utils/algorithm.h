#pragma once

#include <vector>

#include "utils/math.h"

using namespace std;

//Reference: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

vector<SDL_Point> getLinePoints(SDL_Point start, SDL_Point end, int thickness = 1);