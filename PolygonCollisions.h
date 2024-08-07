#pragma once

#ifndef POLYGON_COLLISIONS_H
#define POLYGON_COLLISIONS_H

#include <vector>
#include <algorithm>

struct vec2d
{
	float x;
	float y;

	vec2d(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	// Operator overloading for vector addition and scalar multiplication
	vec2d operator+(const vec2d& other) const {
		return vec2d(x + other.x, y + other.y);
	}

	vec2d operator-(const vec2d& other) const {
		return vec2d(x - other.x, y - other.y);
	}

	vec2d operator*(float scalar) const {
		return vec2d(x * scalar, y * scalar);
	}

	// Function to interpolate between two vectors
	static vec2d lerp(const vec2d& a, const vec2d& b, float t) {
		return a + (b - a) * t;
	}

	// Function to calculate the magnitude of the vector
	float magnitude() const {
		return sqrt(x * x + y * y);
	}
}; 

struct polygon
{
	std::vector<vec2d> p;	// Transformed Points
	vec2d pos;				// Position of shape
	float angle;			// Direction of shape
	std::vector<vec2d> o;	// "Model" of shape							
	bool overlap = false;	// Flag to indicate if overlap has occurred
};


class PolygonCollisions
{
public:
	
	static polygon createQuad(vec2d pos, float angle, float size);

	static polygon createTriangle(vec2d pos, float angle, float size);

	static std::vector<polygon> createWalls(float screen_width, float screen_height, float margin);

	static polygon createWallSegment(vec2d pos, float width, float height);

	static std::vector<polygon> vecShapes;

	// Use edge/diagonal intersections.
	static bool ShapeOverlap_DIAGS_STATIC(polygon& r1, polygon& r2);
	static bool ShapeOverlap_DIAGS(polygon& r1, polygon& r2);
	//static int nMode = 0;
};

#endif 