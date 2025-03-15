#pragma once

#include <cmath>

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

inline double deg2rad(double degrees) {
	return degrees * 4.0 * atan(1.0) / 180.0;
}