#pragma once

#include "SDL.h"

float sRGBInverseCompanding(float x);
float sRGBCompanding(float x);
float lerp(float a, float b, float t);
SDL_Color GradientColorMix(SDL_Color color1, SDL_Color color2, float ratio, float gamma = 0.43);