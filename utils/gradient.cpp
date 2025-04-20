#include "gradient.h"

#pragma once

#include "SDL.h"

float sRGBInverseCompanding(float x) {
	if (x <= 0.04045)
		return x / 12.92;
	else
		return pow((x + 0.055) / 1.055, 2.4);
}

float sRGBCompanding(float x) {
	if (x <= 0.0031308)
		return x * 12.92;
	else
		return 1.055 * pow(x, 1 / 2.4) - 0.055;
}

float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

SDL_Color GradientColorMix(SDL_Color color1, SDL_Color color2, float ratio, float gamma) {

	float r1 = sRGBInverseCompanding((float)color1.r / 255);
	float g1 = sRGBInverseCompanding((float)color1.g / 255);
	float b1 = sRGBInverseCompanding((float)color1.b / 255);

	float r2 = sRGBInverseCompanding((float)color2.r / 255);
	float g2 = sRGBInverseCompanding((float)color2.g / 255);
	float b2 = sRGBInverseCompanding((float)color2.b / 255);

	float r = lerp(r1, r2, ratio);
	float g = lerp(g1, g2, ratio);
	float b = lerp(b1, b2, ratio);

	float brightness = lerp(pow(r1 + g1 + b1, gamma), pow(r2 + g2 + b2, gamma), ratio);
	float intensity = pow(brightness, 1 / gamma);

	if (r + g + b != 0) {
		float factor = intensity / (r + g + b);
		r *= factor;
		g *= factor;
		b *= factor;
	}

	return SDL_Color{
		(Uint8)(sRGBCompanding(r) * 255),
		(Uint8)(sRGBCompanding(g) * 255),
		(Uint8)(sRGBCompanding(b) * 255),
	};
}