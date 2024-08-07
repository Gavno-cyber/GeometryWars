#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include "Engine.h"
#include "GraphicsEngine.h"
#include <dos.h>
#include "GameObjects.h"
#include <algorithm> 
#include <cmath>

using namespace std;

// Define a class 'GameObject'
GameObject::GameObject(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t color)
    : name(name), shape(std::move(shape)), position(position), color(color), speed(speed), health(health) {}

void GameObject::draw() const {
    if (shape) {
        GraphicsEngine::drawPolygon(*shape, color); // Draw the shape associated with this object
    }
}

void GameObject::check_health() {}

vec2d GameObject::get_position() {
    return position;
}

void GameObject::set_position(float x, float y) {
    position.x = x;
    position.y = y;
}

// Define a class named 'Player'
Player::Player(const std::string& name, unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t color)
    : GameObject(name, move(shape), position, speed, health, color) {}

void Player::update_state(float delta) {
    _move(delta);
}

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void Player::check_health() {

    if (health <= 0) {
        schedule_quit_game();
    }
}

void Player::_move(float delta) {

    position = shape->pos;

    vec2d direction(.0f, .0f);

    // Check for input and set direction
    if (is_key_pressed(VK_UP)) direction.y -= 1.0f;
    if (is_key_pressed(VK_DOWN)) direction.y += 1.0f;
    if (is_key_pressed(VK_LEFT)) direction.x -= 1.0f;
    if (is_key_pressed(VK_RIGHT)) direction.x += 1.0f;

    // Normalize the direction vector if it's not zero
    if (direction.x != 0 || direction.y != 0) {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length;
        direction.y /= length;
    }

    // Calculate new position based on speed and delta time
    float new_x = get_position().x + (direction.x * speed * delta);
    float new_y = get_position().y + (direction.y * speed * delta);

    ////Clamp the new position to screen boundaries
    //new_x = clamp(new_x, 0.0 - 100.0f, SCREEN_WIDTH);
    //new_y = clamp(new_y, 0.0 - 100.0f, SCREEN_HEIGHT);

    // Get the current position
    vec2d current_position = get_position();

    // Smoothly interpolate to the new position
    vec2d smoothed_position = vec2d::lerp(current_position, vec2d(new_x, new_y), 0.3f);

    // Set the smoothed position
    set_position(smoothed_position.x, smoothed_position.y);

    //set_position(new_x, new_y);

    vec2d new_pos(position.x - SCREEN_WIDTH / 2, position.y - SCREEN_HEIGHT / 2);
    Camera::move(new_pos, delta);

    shape->pos = position;

    for (int i = 0; i < shape->o.size(); i++)
        shape->p[i] =
    {	// 2D Rotation Transform + 2D Translation
        (shape->o[i].x * cosf(shape->angle)) - (shape->o[i].y * sinf(shape->angle)) + shape->pos.x,
        (shape->o[i].x * sinf(shape->angle)) + (shape->o[i].y * cosf(shape->angle)) + shape->pos.y,
    };
}

void Player::shoot(float delta) {
    
    // Player Weapon Fire
    bool bCanFire = false;

    fPlayerGunReloadTime -= delta;

    if (fPlayerGunReloadTime <= 0.0f)
    {
        bCanFire = true;
    }

    fPlayerGunTemp -= delta * 10.0f;

    if (fPlayerGunTemp < 0) fPlayerGunTemp = 0;

    // Get cursor position
    float cursorX = get_cursor_x() + Camera::position.x;
    float cursorY = get_cursor_y() + Camera::position.y;

    // Calculate direction from player position to mouse cursor
    vec2d direction(cursorX - position.x, cursorY - position.y);

    // Normalize the direction vector if it's not zero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.0f) {  // Avoid division by zero
        direction.x /= length;  // Normalize x component
        direction.y /= length;  // Normalize y component
    }

    const double Pi = acos(-1);
    shape->angle = (180.0f * atan2(direction.y, direction.x)) / Pi * 0.0175f;

    if (is_mouse_button_pressed(0))
    {
        if (bCanFire && fPlayerGunTemp < 80.0f)
        {
            fPlayerGunReloadTime = fPlayerGunReload;
            fPlayerGunTemp += 1.0f;
            if (fPlayerGunTemp > 100.0f) fPlayerGunTemp = 100.0f;

            float half_spread = spread_angle / 2.0f;
            // Calculate the angle increment
            float angle_increment = spread_angle / (number_of_bullets - 1);

            for (int i = 0; i < number_of_bullets; i++) {
                // Calculate the current angle
                float angle = half_spread - (i * angle_increment);
                // Convert angle to radians
                float radians = angle * (acos(-1) / 180.0f);

                // Calculate new direction based on the angle
                vec2d bullet_direction = {
                    direction.x * cos(radians) - direction.y * sin(radians),
                    direction.x * sin(radians) + direction.y * cos(radians)
                };

                // Add bullet to the list
                listPlayerBullets.push_back({ vec2d(position.x, position.y), bullet_direction * 1000.0f });
            }

        }
    }
}

// Define a class 'Enemy'
Enemy::Enemy(const std::string& name, std::unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t value, uint32_t color, Player& player)
    : GameObject(name, move(shape), position, speed, health, color), player(player), _value(value) {}

void Enemy::update_state(float delta)
{
}


TriangleEnemy::TriangleEnemy(const std::string& name, std::unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t value, uint32_t color, Player& player)
    : Enemy(name, move(shape), position, speed, health, value, color, player) {}

void TriangleEnemy::_move(float delta) {
    // Calculate direction from player position to mouse cursor
    vec2d direction(player.get_position().x - position.x, player.get_position().y - position.y);

    // Normalize the direction vector if it's not zero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.0f) {  // Avoid division by zero
        direction.x /= length;  // Normalize x component
        direction.y /= length;  // Normalize y component
    }

    // Calculate new position based on speed and delta time
    float new_x = get_position().x + (direction.x * speed * delta);
    float new_y = get_position().y + (direction.y * speed * delta);

    // Clamp the new position to screen boundaries
    new_x = clamp(new_x, 0.0f, SCREEN_WIDTH - 50);
    new_y = clamp(new_y, 0.0f, SCREEN_HEIGHT - 50);

    // Smoothly interpolate to the new position
    vec2d smoothed_position = vec2d::lerp(get_position(), vec2d(new_x, new_y), 0.3f);

    // Set the smoothed position
    set_position(smoothed_position.x, smoothed_position.y);

    shape->pos = position;

    for (int i = 0; i < shape->o.size(); i++)
        shape->p[i] =
    {	// 2D Rotation Transform + 2D Translation
        (shape->o[i].x * cosf(shape->angle)) - (shape->o[i].y * sinf(shape->angle)) + shape->pos.x,
        (shape->o[i].x * sinf(shape->angle)) + (shape->o[i].y * cosf(shape->angle)) + shape->pos.y,
    };
}

void TriangleEnemy::update_state(float delta) {
    TriangleEnemy::_move(delta);
}

SquareEnemy::SquareEnemy(const std::string& name, std::unique_ptr<polygon> shape, vec2d position, float speed, vec2d direction, uint32_t health, uint32_t value, uint32_t color, Player& player)
    : Enemy(name, move(shape), position, speed, health, color, value, player), direction(direction) {}

void SquareEnemy::_move(float delta) {
    // Normalize the direction vector if it's not zero

    if (position.y <= 0 || position.y >= SCREEN_HEIGHT) {
        direction = vec2d(direction.x, -direction.y);
    }

    if (position.x <= 0 || position.x >= SCREEN_WIDTH) {
        direction = vec2d(-direction.x, direction.y);
    }

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.0f) {  // Avoid division by zero
        direction.x /= length;  // Normalize x component
        direction.y /= length;  // Normalize y component
    }

    // Calculate new position based on speed and delta time
    float new_x = get_position().x + (direction.x * speed * delta);
    float new_y = get_position().y + (direction.y * speed * delta);

    // Clamp the new position to screen boundaries
    //new_x = clamp(new_x, 0.0f, SCREEN_WIDTH - 50);
    //new_y = clamp(new_y, 0.0f, SCREEN_HEIGHT - 50);

    // Smoothly interpolate to the new position
    vec2d smoothed_position = vec2d::lerp(get_position(), vec2d(new_x, new_y), 0.3f);

    // Set the smoothed position
    set_position(smoothed_position.x, smoothed_position.y);

    shape->pos = position;

    for (int i = 0; i < shape->o.size(); i++)
        shape->p[i] =
    {	// 2D Rotation Transform + 2D Translation
        (shape->o[i].x * cosf(shape->angle)) - (shape->o[i].y * sinf(shape->angle)) + shape->pos.x,
        (shape->o[i].x * sinf(shape->angle)) + (shape->o[i].y * cosf(shape->angle)) + shape->pos.y,
    };
}

void SquareEnemy::update_state(float delta) {
    _move(delta);
}


RotatingEnemy::RotatingEnemy(const std::string& name, std::unique_ptr<polygon> shape, vec2d position, float speed, uint32_t health, uint32_t value, uint32_t color, Player& player)
    : Enemy(name, move(shape), position, speed, health, color, value, player) {}

void RotatingEnemy::_move(float delta) {
    // Calculate direction from player position to mouse cursor
    vec2d direction(player.get_position().x - position.x, player.get_position().y - position.y);

    // Normalize the direction vector if it's not zero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.0f) {  // Avoid division by zero
        direction.x /= length;  // Normalize x component
        direction.y /= length;  // Normalize y component
    }

    // Calculate new position based on speed and delta time
    float new_x = get_position().x + (direction.x * speed * delta);
    float new_y = get_position().y + (direction.y * speed * delta);

    // Clamp the new position to screen boundaries
    new_x = clamp(new_x, 0.0f, SCREEN_WIDTH - 50);
    new_y = clamp(new_y, 0.0f, SCREEN_HEIGHT - 50);

    // Smoothly interpolate to the new position
    vec2d smoothed_position = vec2d::lerp(get_position(), vec2d(new_x, new_y), 0.3f);

    // Set the smoothed position
    set_position(smoothed_position.x, smoothed_position.y);

    shape->pos = position;

    shape->angle -= 2.0f * delta;

    for (int i = 0; i < shape->o.size(); i++)
        shape->p[i] =
    {	// 2D Rotation Transform + 2D Translation
        (shape->o[i].x * cosf(shape->angle)) - (shape->o[i].y * sinf(shape->angle)) + shape->pos.x,
        (shape->o[i].x * sinf(shape->angle)) + (shape->o[i].y * cosf(shape->angle)) + shape->pos.y,
    };
}

void RotatingEnemy::update_state(float delta) {
    RotatingEnemy::_move(delta);
}

EnemySpawner::EnemySpawner(float spawnDelay, Player& player)
    : spawnDelay(spawnDelay), player(player), lastSpawnTime(0.0f) {}

void EnemySpawner::update(float delta) {
    if (spawnDelay > minSpawnTime) {
        spawnDelay -= spawnDecreaseRate * delta;
        if (spawnDelay < minSpawnTime) {
            spawnDelay = minSpawnTime;
        }
    }
    
    lastSpawnTime += delta;
    if (lastSpawnTime >= spawnDelay) {
        spawn_enemy();
        lastSpawnTime = 0.0f; // Reset the spawn timer
    }
}

const std::list<std::unique_ptr<Enemy>>& EnemySpawner::get_enemies() const {
    return enemies;
}

void EnemySpawner::remove_all_enemies() {
    // Create and add the new enemy to the list
    enemies.clear();
}

void EnemySpawner::remove_enemy() {
    enemies.remove_if([&](std::unique_ptr<Enemy>& e) {return e->health <= 0; });
}

void EnemySpawner::spawn_enemy() {
    // Define enemy properties
    std::string name = "Enemy_" + std::to_string(enemies.size());

    vec2d position(get_random_spawn_position());

    int enemyType = rand() % 3; // Assuming we have two types: Goblin (0) and Orc (1)

    //std::unique_ptr<Enemy> enemy;

    if (enemyType == 0) {

        polygon s = PolygonCollisions::createQuad(position, 0.0f, 20.0f);

        std::unique_ptr<polygon> shape(make_unique<polygon>(s)); // Создание уникального указателя на Shape

        float speed = 500.0f; // Set desired speed
        uint32_t color = 0xFFFFFF; // Example color 

        //У этого персонажа цвет синим становится с какого-то перепугу
        std::unique_ptr<SquareEnemy> enemy = std::make_unique<SquareEnemy>(name, std::move(shape), position, speed, vec2d(player.get_position().x - position.x, player.get_position().y - position.y), 1, 100, color, player);
        enemies.push_back(std::move(enemy));
    }
    else if (enemyType == 1) {
    
        polygon s = PolygonCollisions::createTriangle(position, 0.0f, 20.0f);
    
        float speed = 300.0f; // Set desired speed
        uint32_t color = 0xFFFFFF; // Example color 
        
        for (int i = 0; i < 3; i++)
        {	// 2D Rotation Transform + 2D Translation
            std::unique_ptr<polygon> shape(make_unique<polygon>(s)); // Создание уникального указателя на Shape

            std::unique_ptr<TriangleEnemy> enemy = std::make_unique<TriangleEnemy>(name, std::move(shape), position + vec2d(i * 20, i * 40), speed, 1, 50, color, player);
            
            enemies.push_back(std::move(enemy));
        };
    }
    else {
        polygon s = PolygonCollisions::createQuad(position, 0.0f, 20.0f);
    
        std::unique_ptr<polygon> shape(make_unique<polygon>(s)); // Создание уникального указателя на Shape
    
        float speed = 500.0f; // Set desired speed
        uint32_t color = 0xFFFFFF; // Example color

        //У этого персонажа тоже цвет синим становится с какого-то перепугу
        std::unique_ptr<RotatingEnemy> enemy = std::make_unique<RotatingEnemy>(name, std::move(shape), position, speed, 1, 250, color, player);
        enemies.push_back(std::move(enemy));
    }

    // Create and add the new enemy to the list
    //enemies.push_back(std::move(enemy));
}

vec2d EnemySpawner::get_random_spawn_position() {
    // Generate random x and y positions within the screen boundaries
    vec2d playerPos = player.get_position();

    float angle = static_cast<float>(rand()) / RAND_MAX * 2 * acos(-1);; // Random angle in radians
    float minSpawnDistance = 700.0f; // Adjust as needed

    float x = playerPos.x + cos(angle) * minSpawnDistance;
    float y = playerPos.y + sin(angle) * minSpawnDistance;

    // Ensure the spawn position is within screen bounds
    x = clamp(x, 0.0f, SCREEN_WIDTH);
    y = clamp(y, 0.0f, SCREEN_HEIGHT);
    return vec2d(x, y);
}