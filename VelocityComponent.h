#pragma once
#ifndef VELOCITY_COMPONENT
#define VELOCITY_COMPONENT

#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include "PhysicsEngine.h"

using namespace std;

class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0);

    Vector2 operator+(const Vector2& other) const;

    Vector2 operator-(const Vector2& other) const;

    Vector2 operator*(float scalar) const;

    Vector2 operator/(float scalar) const;

    float length() const;

    Vector2 normalized() const;
};

class VelocityComponent {
public:

    Vector2 velocity;

    float max_speed;
    float acceleration;
    float friction;
    float lerpFactor; // Smooth movement factor

    VelocityComponent(Vector2 velocity = Vector2(), float max_speed = 1, float acceleration = 0.1f, float friction = 0.1f, float lerpFactor = 0.1f);

    void accelerateVelocity(Vector2 velocity, float _acceleration);

    void accelerateInDirection(Vector2 direction);

    void move();

private:
    Vector2 lerp(const Vector2& start, const Vector2& end, float t);
};





//// Derived class for Wall
//class Wall : public GameObject {
//public:
//    Wall(const string& name, unique_ptr<Shape> shape)
//        : GameObject(name, move(shape)) {}
//
//    // Additional wall-specific methods can be added here
//};

#endif // VELOCITY_COMPONENT