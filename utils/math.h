#pragma once

#include <cmath>
#include "textures.h"

#define M_E 2.71828182845904523536

struct vec2_t {
	float x;
	float y;

	vec2_t() : x(0), y(0) {} // Default constructor

	vec2_t(float x, float y) : x(x), y(y) {}

	vec2_t operator+(const vec2_t& other) {
		return vec2_t(x + other.x, y + other.y);
	}

	vec2_t operator-(const vec2_t& other) {
		return vec2_t(x - other.x, y - other.y);
	}

	vec2_t operator*(const float& scalar) {
		return vec2_t(x * scalar, y * scalar);
	}

	vec2_t operator/(const float& scalar) {
		return vec2_t(x / scalar, y / scalar);
	}

	bool operator==(const vec2_t& other) {
		return x == other.x && y == other.y;
	}
};

inline float deg2rad(float degrees) {
	return degrees * 4.0f * atan(1.0f) / 180.0f;
}