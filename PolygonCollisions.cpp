#include <vector>
#include <algorithm>
#include "PolygonCollisions.h"


polygon PolygonCollisions::createQuad(vec2d pos, float angle, float size) {
	// Create Quad
	polygon s;
	s.pos = pos;
	s.angle = angle;
	s.o.push_back({ -size, -size });
	s.o.push_back({ -size, +size });
	s.o.push_back({ +size, +size });
	s.o.push_back({ +size, -size });
	s.p.resize(4);
	return s;
};

polygon PolygonCollisions::createTriangle(vec2d pos, float angle, float size) {
	// Create Quad
	polygon s;
	float fTheta = 3.14159f * 2.0f / 3.0f;
	s.pos = pos;
	s.angle = angle;
	for (int i = 0; i < 3; i++)
	{
		s.o.push_back({ size * cosf(fTheta * i), size * sinf(fTheta * i) });
		s.p.push_back({ size * cosf(fTheta * i), size * sinf(fTheta * i) });
	}
	return s;
};

// Function to create the walls for the walking area
std::vector<polygon> PolygonCollisions::createWalls(float screen_width, float screen_height, float margin) {
	std::vector<polygon> walls;

	// Define wall thickness
	float wallThickness = 15.0f;

	// Create walls for the room
	walls.push_back(createWallSegment({ screen_width / 2.0f, wallThickness / 2.0f }, screen_width, wallThickness)); // Bottom Wall
	walls.push_back(createWallSegment({ screen_width / 2.0f, screen_height - wallThickness / 2.0f }, screen_width, wallThickness)); // Top Wall
	walls.push_back(createWallSegment({ wallThickness / 2.0f, screen_height / 2.0f }, wallThickness, screen_height)); // Left Wall
	walls.push_back(createWallSegment({ screen_width - wallThickness / 2.0f, screen_height / 2.0f }, wallThickness, screen_height)); // Right Walls

	return walls;
}

// Function to create a wall segment (rectangle)
polygon PolygonCollisions::createWallSegment(vec2d pos, float width, float height) {
	polygon wall;
	wall.pos = pos;
	wall.angle = 0;
	// Define corners for the wall segment
	wall.o.push_back({ -width / 2.0f, -height / 2.0f });
	wall.o.push_back({ -width / 2.0f, height / 2.0f });
	wall.o.push_back({ width / 2.0f, height / 2.0f });
	wall.o.push_back({ width / 2.0f, -height / 2.0f });
	wall.p.resize(4);
	return wall;
}

bool PolygonCollisions::ShapeOverlap_DIAGS_STATIC(polygon& r1, polygon& r2)
{
	polygon* poly1 = &r1;
	polygon* poly2 = &r2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = &r2;
			poly2 = &r1;
		}

		// Check diagonals of this polygon...
		for (int p = 0; p < poly1->p.size(); p++)
		{
			vec2d line_r1s = poly1->pos;
			vec2d line_r1e = poly1->p[p];

			vec2d displacement = { 0,0 };

			// ...against edges of this polygon
			for (int q = 0; q < poly2->p.size(); q++)
			{
				vec2d line_r2s = poly2->p[q];
				vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

				// Standard "off the shelf" line segment intersection
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
				{
					displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
					displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
				}
			}

			r1.pos.x += displacement.x * (shape == 0 ? -1 : +1);
			r1.pos.y += displacement.y * (shape == 0 ? -1 : +1);
		}
	}

	// Cant overlap if static collision is resolved
	return false;
}

// Use edge/diagonal intersections.
bool PolygonCollisions::ShapeOverlap_DIAGS(polygon& r1, polygon& r2)
{
	polygon* poly1 = &r1;
	polygon* poly2 = &r2;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = &r2;
			poly2 = &r1;
		}

		// Check diagonals of polygon...
		for (int p = 0; p < poly1->p.size(); p++)
		{
			vec2d line_r1s = poly1->pos;
			vec2d line_r1e = poly1->p[p];

			// ...against edges of the other
			for (int q = 0; q < poly2->p.size(); q++)
			{
				vec2d line_r2s = poly2->p[q];
				vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

				// Standard "off the shelf" line segment intersection
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

				if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
				{
					return true;
				}
			}
		}
	}
	return false;
}