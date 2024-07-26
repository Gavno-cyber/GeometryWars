#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include "VelocityComponent.h"

using namespace std;


Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
    return Vector2(x / scalar, y / scalar);
}

float Vector2::length() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    float len = length();
    if (len > 0) {
        return *this / len;
    }
    return Vector2(0, 0);
}


VelocityComponent::VelocityComponent(Vector2 velocity, float max_speed, float acceleration, float friction, float lerpFactor)
    : velocity(velocity), acceleration(acceleration), max_speed(max_speed), friction(friction), lerpFactor(lerpFactor) {}

void VelocityComponent::accelerateVelocity(Vector2 _velocity, float _acceleration) {
    velocity = lerp(velocity, _velocity, _acceleration);
}

void VelocityComponent::accelerateInDirection(Vector2 direction) {
    VelocityComponent::accelerateVelocity(direction * max_speed, acceleration);
}

void VelocityComponent::move()
{
    //characterBody.set_position(velocity.x + characterBody.get_position().x, velocity.y + characterBody.get_position().y);
}

Vector2 VelocityComponent::lerp(const Vector2& start, const Vector2& end, float t) {
    return start + (end - start) * t;
}