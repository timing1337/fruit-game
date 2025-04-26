#pragma once

#include <cmath>
#include "textures.h"

#define M_E 2.71828182845904523536

struct vec2_t {
	int x;
	int y;

	vec2_t() : x(0), y(0) {} // Default constructor

	vec2_t(int x, int y) : x(x), y(y) {}

	vec2_t operator+(const vec2_t& other) {
		return vec2_t(x + other.x, y + other.y);
	}

	vec2_t operator-(const vec2_t& other) {
		return vec2_t(x - other.x, y - other.y);
	}

	vec2_t operator*(const int& scalar) {
		return vec2_t(x * scalar, y * scalar);
	}

	vec2_t operator/(const int& scalar) {
		return vec2_t(x / scalar, y / scalar);
	}

	bool operator==(const vec2_t& other) {
		return x == other.x && y == other.y;
	}
};

inline float deg2rad(float degrees) {
	return degrees * 4.0f * atan(1.0f) / 180.0f;
}

inline bool isPointInRect(SDL_Point point, SDL_Point position, GameTexture* gameTexture) {
	return point.x >= position.x && point.x <= position.x + gameTexture->width && point.y >= position.y && point.y <= position.y + gameTexture->height;
}

inline bool isPointInRect(SDL_Point point, SDL_Point position, SDL_Point size) {
	return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}