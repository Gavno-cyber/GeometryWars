#include <cmath>
#include <iostream>
#include <variant>
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"

#include <algorithm>
#include <vector>
#include <tuple>
#include <utility> // Äëÿ std::pair

Point::Point(uint32_t x, uint32_t y) : x(x), y(y) {}

uint32_t Point::squared_distance(const Point& other) const {
    uint32_t dX = x - other.x;
    uint32_t dY = y - other.y;
    return dX * dX + dY * dY;
}

Line::Line(Point start, Point end) : start(start), end(end) {}

bool Line::onLine(const Point& p) const {
    // Handle vertical lines
    if (start.x == end.x) {
        return p.x == start.x;
    }
    uint32_t m = (end.y - start.y) / (end.x - start.x);
    uint32_t b = start.y - m * start.x;
    return p.y == m * p.x + b;
}

void Shape::draw(Point position, uint32_t color) const {}; // Pure virtual function for drawing

Triangle::Triangle(Point top, Point left, Point right)
    : top(top), left(left), right(right) {}

void Triangle::draw(Point position, uint32_t color) const {
    GraphicsEngine::drawPolygon(std::vector<std::pair<int, int>> {
        {top.x, top.y},  // Vertex 1
        { left.x, left.y },  // Vertex 2
        { right.x, right.y }   // Vertex 3
    }, color);
};

// Circle class
Circle::Circle(const Point& center, uint32_t radius) : center(center), radius(radius) {}

void Circle::draw(Point position, uint32_t color) const {
    GraphicsEngine::drawCircle(position.x, position.y, radius, color);
}

// Rectangle class
Rectangle::Rectangle(Point bottom_left, uint32_t width, uint32_t height)
    : bottom_left(bottom_left), width(width), height(height) {}

// Method to get the top-right corner of the rectangle
Point Rectangle::top_right() const {
    return Point(bottom_left.x + width, bottom_left.y + height);
}

void Rectangle::draw(Point position, uint32_t color) const {
    GraphicsEngine::drawRectangle(position.x, position.y, width, height, color);
}

//// Free function to check if two circles intersect
//bool intersect(const Circle& a, const Circle& b) {
//    uint32_t sqr_dist = a.center.squared_distance(b.center);
//    return sqr_dist <= std::pow(a.radius + b.radius, 2);
//}
//
//// Free function to check if a line intersects with a circle
//bool intersect(const Line& line, const Circle& circle) {
//    uint32_t A = line.end.y - line.start.y;
//    uint32_t B = line.start.x - line.end.x;
//    uint32_t C = A * line.start.x + B * line.start.y;
//
//    uint32_t D = std::pow(A, 2) + std::pow(B, 2);
//
//    // Handle potential division by zero
//    if (D == 0) {
//        return false; // The line is degenerate (start and end are the same)
//    }
//
//    uint32_t x0 = circle.center.x;
//    uint32_t y0 = circle.center.y;
//    uint32_t r = circle.radius;
//
//    // Calculate the distance from the circle center to the line
//    uint32_t distance_squared = (A * x0 + B * y0 - C) * (A * x0 + B * y0 - C) / D;
//
//    return distance_squared <= r * r;
//}
//
//// Free function to check if two lines intersect
//bool intersect(const Line& a, const Line& b) {
//    uint32_t det = (a.end.x - a.start.x) * (b.end.y - b.start.y) - (a.end.y - a.start.y) * (b.end.x - b.start.x);
//
//    // Lines are parallel
//    if (det == 0) {
//        return false;
//    }
//
//    uint32_t t = ((a.start.x - b.start.x) * (b.end.y - b.start.y) - (a.start.y - b.start.y) * (b.end.x - b.start.x)) / det;
//    uint32_t u = ((a.start.x - b.start.x) * (a.end.y - a.start.y) - (a.start.y - b.start.y) * (a.end.x - a.start.x)) / det;
//
//    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
//}
//
//// Function to check if two rectangles intersect
//bool intersect(const Rectangle& a, const Rectangle& b) {
//    // Check if one rectangle is to the left of the other
//    if (a.bottom_left.x >= b.top_right().x || b.bottom_left.x >= a.top_right().x) {
//        return false;
//    }
//
//    // Check if one rectangle is above the other
//    if (a.bottom_left.y >= b.top_right().y || b.bottom_left.y >= a.top_right().y) {
//        return false;
//    }
//
//    return true; // Rectangles intersect
//}
//
//bool pointInRectangle(const Point& p, const Rectangle& rect) {
//    return (p.x >= rect.bottom_left.x && p.x <= rect.top_right().x &&
//        p.y >= rect.bottom_left.y && p.y <= rect.top_right().y);
//}
//
//// Function to check if a circle intersects with a rectangle
//bool intersect(const Circle& circle, const Rectangle& rect) {
//    // Find the closest point on the rectangle to the circle's center
//    double closestX = std::max(rect.bottom_left.x, std::min(circle.center.x, rect.top_right().x));
//    double closestY = std::max(rect.bottom_left.y, std::min(circle.center.y, rect.top_right().y));
//
//    // Calculate the distance from the circle's center to this closest point
//    double distanceX = circle.center.x - closestX;
//    double distanceY = circle.center.y - closestY;
//
//    // If the distance is less than the radius, they intersect
//    return (distanceX * distanceX + distanceY * distanceY) < (circle.radius * circle.radius);
//}
//
//// Function to check if a line intersects with a rectangle
//bool intersect(const Line& line, const Rectangle& rect) {
//    // Check against all four edges of the rectangle
//    Line topEdge(Point(rect.bottom_left.x, rect.top_right().y), Point(rect.top_right().x, rect.top_right().y));
//    Line bottomEdge(Point(rect.bottom_left.x, rect.bottom_left.y), Point(rect.top_right().x, rect.bottom_left.y));
//    Line leftEdge(Point(rect.bottom_left.x, rect.bottom_left.y), Point(rect.bottom_left.x, rect.top_right().y));
//    Line rightEdge(Point(rect.top_right().x, rect.bottom_left.y), Point(rect.top_right().x, rect.top_right().y));
//
//    return intersect(line, topEdge) || intersect(line, bottomEdge) ||
//        intersect(line, leftEdge) || intersect(line, rightEdge);
//};