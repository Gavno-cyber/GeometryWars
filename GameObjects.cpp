#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include "Engine.h"
#include "PhysicsEngine.h"
#include <dos.h>
#include "GameObjects.h"


using namespace std;



// Define a class 'GameObject'
GameObject::GameObject(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t color)
    : name(name), shape(std::move(shape)), position(position), color(color) {}

void GameObject::draw() const {
    if (shape) {
        shape->draw(position, color); // Draw the shape associated with this object
    }
}

Point GameObject::get_position() {
    return position;
}

void GameObject::set_position(uint32_t x, uint32_t y) {
    position.x = x;
    position.y = y;
}


// Define a class named 'Player'
Player::Player(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t& color, VelocityComponent& velocity_component, std::vector<Bullet*> bullets)
    : GameObject(name, move(shape), position, color), velocity_component(velocity_component), bullets(bullets) {}

void Player::update_state() {
    _move();
    handle_bullets();
}

void Player::_move() {
    velocity_component.velocity = Vector2(0, 0);
    Vector2 direction = move_input();
    if (direction.x != 0 || direction.y != 0) {
        velocity_component.accelerateInDirection(direction);
        Vector2 velocity = velocity_component.velocity;
        set_position(velocity.x + get_position().x, velocity.y + get_position().y);
    }
    //velocity_component.move(std::unique_ptr<Player> player);
}

Vector2 Player::move_input() {
    Vector2 direction = Vector2(.0f, .0f);
    //VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN
    if (is_key_pressed(VK_RIGHT)) {
        direction.x += 1;
    }
    else if (is_key_pressed(VK_LEFT)) {
        direction.x -= 1;
    }

    if (is_key_pressed(VK_UP)) {
        direction.y -= 1;
    }
    else if (is_key_pressed(VK_DOWN)) {
        direction.y += 1;
    }
    direction = direction.normalized();
    return direction;
}

void Player::shoot() {


    if (is_mouse_button_pressed(0)) {

        uint32_t bulletColor = 0x0FFFF0; // Пример цвета (зеленый)

        std::string name = "Bullet"; // Инициализация playerName
        uint32_t width(5), height(5);
        std::unique_ptr<Shape> shape = std::make_unique<Rectangle>(position, width, height); // Создание уникального указателя на Shape
        VelocityComponent velocity_component = VelocityComponent();
        
        Vector2 direction = Vector2(0, -1);

        //direction.x = get_cursor_x();
        //direction.y = get_cursor_y();

        direction = direction.normalized();

        Bullet* bullet = new Bullet(name, std::move(shape), position, bulletColor, velocity_component, direction);

        bullets.push_back(bullet);
    }
    //VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN

}

void Player::handle_bullets() {
    if (bullets.size() > 0) {
        for (int i = 0; i < bullets.size() - 1; i++) {
            bullets[i]->_move();
        }
    }
}

void Player::draw_bullets() {
    if (bullets.size() > 0) {
        for (int i = 0; i < bullets.size() - 1; i++) {
            bullets[i]->draw();
        }
    }
}


// Define a class 'Enemy'
Enemy::Enemy(const std::string& name, std::unique_ptr<Shape> shape, Point position, uint32_t& color)
    : GameObject(name, move(shape), position, color) {}

void Enemy::update_state() {
    draw();
}


// Define a class 'Bullet'
Bullet::Bullet(const std::string& name, unique_ptr<Shape> shape, Point position, uint32_t& color, VelocityComponent& velocity_component, Vector2 direction)
    : GameObject(name, move(shape), position, color), velocity_component(velocity_component), direction(direction) {}

void Bullet::update_state() {
    draw();
}

void Bullet::_move() {
    velocity_component.velocity = Vector2(0, 0);
    velocity_component.accelerateInDirection(direction);
    Vector2 velocity = velocity_component.velocity;
    set_position(velocity.x + get_position().x, velocity.y + get_position().y);
}

// Additional player-specific methods can be added here

//// Derived class for Wall
//class Wall : public GameObject {
//public:
//    Wall(const string& name, unique_ptr<Shape> shape)
//        : GameObject(name, move(shape)) {}
//
//    // Additional wall-specific methods can be added here
//};