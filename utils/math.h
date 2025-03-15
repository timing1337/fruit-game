#pragma once

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
