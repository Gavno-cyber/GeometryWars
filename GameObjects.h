#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include "PhysicsEngine.h"
#include "VelocityComponent.h"

using namespace std;

// Define a class GameObject    
class GameObject {
protected:
    std::string name;
    unique_ptr<Shape> shape; // Unique pointer to a shape
    Point position;
    uint32_t color;

public:
    GameObject(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t color);

    Point get_position();

    void set_position(uint32_t x, uint32_t y);

    virtual void draw() const;

    virtual ~GameObject() = default; // Virtual destructor for cleanup
};


// Define a class named 'Player'
class Player : public GameObject {
public:
    std::vector<class Bullet*> bullets;

    VelocityComponent velocity_component;

    Player(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t& color, VelocityComponent& velocity_component, std::vector<Bullet*> bullets);

    void update_state();

    void _move();

    void shoot();

    void handle_bullets();

    void draw_bullets();

    Vector2 move_input();
    // Additional player-specific methods can be added here
};



// Define a class named 'Enemy'
class Enemy : public GameObject {
public:
    Enemy(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t& color);

    void update_state();
};


// Define a class named 'Bullet'
class Bullet : public GameObject {
public:
    VelocityComponent velocity_component;
    Vector2 direction;

    Bullet(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t& color, VelocityComponent& velocity_component, Vector2 direction);

    void update_state();

    void _move();
};

//// Derived class for Wall
//class Wall : public GameObject {
//public:
//    Wall(const string& name, unique_ptr<Shape> shape)
//        : GameObject(name, move(shape)) {}
//
//    // Additional wall-specific methods can be added here
//};

#endif // GAMEOBJECT