#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <algorithm> 
#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include "PolygonCollisions.h"
#include "VelocityComponent.h"
#include <list>

using namespace std;

// Define a class GameObject    
class GameObject {
protected:
    std::string name;

    vec2d position;

public:
    GameObject(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t color);

    unique_ptr<polygon> shape; // Unique pointer to a shape

    float speed;

    uint32_t health;

    uint32_t color;

    vec2d get_position();

    void set_position(float x, float y);

    virtual void draw() const;

    virtual void check_health();

    virtual ~GameObject() = default; // Virtual destructor for cleanup
};


// Define a class named 'Player'
class Player : public GameObject {
public:
    struct sBullet
    {
        vec2d pos;
        vec2d vel;
        bool remove = false;
    };

    std::list<sBullet> listPlayerBullets;

    float spread_angle = 10.0f;
    uint32_t number_of_bullets = 2;
    float fPlayerGunTemp = 0.0f;
    float fPlayerGunReload = 0.1f;
    float fPlayerGunReloadTime = 0.0f;

    Player(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t color);

    void update_state(float delta);

    void _move(float delta);

    void shoot(float delta);

    void check_health();
};


// Define a class named 'Enemy'
class Enemy : public GameObject {
public:
    uint32_t _value;
    Player& player;

    bool remove = false;    

    Enemy(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t value, uint32_t color, Player& player);

    virtual void update_state(float delta) = 0;

    virtual void _move(float delta) = 0;
};

// Define a class named 'Enemy'
class TriangleEnemy: public Enemy {
public:

    TriangleEnemy(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t value, uint32_t color, Player& player);

    void _move(float delta);

    void update_state(float delta);
};

// Define a class named 'Enemy'
class SquareEnemy : public Enemy {
public:
    vec2d direction;

    SquareEnemy(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, vec2d direction, uint32_t health, uint32_t value, uint32_t color, Player& player);

    void _move(float);

    void update_state(float);
};

// Define a class named 'Enemy'
class RotatingEnemy : public Enemy {
public:

    RotatingEnemy(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t value, uint32_t color, Player& player);

    void _move(float);

    void update_state(float);
};


class EnemySpawner {
public:
    EnemySpawner(float spawnDelay, Player& player);

    void update(float delta);

    const std::list<std::unique_ptr<Enemy>>& get_enemies() const;

    void remove_all_enemies();

    void remove_enemy();

private:
    void spawn_enemy();

    vec2d get_random_spawn_position();

    float minSpawnTime = 0.03f, spawnDecreaseRate = 0.03f;
    float spawnDelay; // Time delay between spawns
    float lastSpawnTime; // Time since last spawn
    Player& player; // Reference to the player object
    std::list<std::unique_ptr<Enemy>> enemies; // List of spawned enemies
};


#endif // GAMEOBJECT