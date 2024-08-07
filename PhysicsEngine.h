#pragma once

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <cstdint>
#include <cmath>

#include <cmath>
#include <iostream>
#include <variant>
#include "GraphicsEngine.h"

#include <algorithm>
#include <vector>
#include <tuple>
#include <utility> // Äëÿ std::pair

// Point class
class Point {
public:
    int x, y;

    Point(int x, int y);
    int squared_distance(const Point& other) const;
};

// Line class
class Line {
public:
    Point start, end;

    Line(Point start, Point end);
    bool onLine(const Point& p) const;
};

// Base class for shapes
class Shape {
public:
    virtual void draw(Point position, uint32_t color) const = 0; // Pure virtual function for drawing
    virtual ~Shape() = default; // Virtual destructor for cleanup
};

// Derived class for Triangle
class Triangle : public Shape {
private:
    Point top; // Vertex 1
    Point left; // Vertex 2
    Point right; // Vertex 3

public:
    Triangle(Point top, Point left, Point right);
    void draw(Point position, uint32_t color) const override; // Implement draw method
};

// Circle class
class Circle : public Shape {
public:
    Point center;
    uint32_t radius;

    Circle(const Point& center, uint32_t radius);
    void draw(Point position, uint32_t color) const override; // Implement draw method
};

// Rectangle class
class Rectangle : public Shape {
public:
    Point bottom_left; // Bottom-left corner of the rectangle
    uint32_t width, height;

    Rectangle(Point bottom_left, uint32_t width, uint32_t height);
    Point top_right() const; // Method to get the top-right corner of the rectangle
    void draw(Point position, uint32_t color) const override; // Implement draw method
};

// Free function to check if two circles intersect
//bool intersect(const Circle& a, const Circle& b);
//
//// Free function to check if a line intersects with a circle
//bool intersect(const Line& line, const Circle& circle);
//
//// Free function to check if two lines intersect
//bool intersect(const Line& a, const Line& b);
//
//// Function to check if two rectangles intersect
//bool intersect(const Rectangle& a, const Rectangle& b);
//
//// Function to check if a point is inside a rectangle
//bool pointInRectangle(const Point& p, const Rectangle& rect);

#endif // PHYSICS_ENGINE_H