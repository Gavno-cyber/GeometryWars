#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"
#include "GameObjects.h"

using namespace std;

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

Player* player = nullptr;

// initialize game data in this function
void initialize()
{
    Point playerPosition(200, 100); // Создание объекта Point
    uint32_t playerColor = 0x00FF00; // Пример цвета (зеленый)

    std::string playerName = "Hero"; // Инициализация playerName
    uint32_t width(50), height(50);
    std::unique_ptr<Shape> playerShape = std::make_unique<Rectangle>(playerPosition, width, height); // Создание уникального указателя на Shape
    VelocityComponent velocity_component = VelocityComponent();
    std::vector<class Bullet*> bullets {};
    // Создание объекта Player
    player = new Player(playerName, std::move(playerShape), playerPosition, playerColor, velocity_component, bullets);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (player) {
        player->update_state();
        player->shoot();// Используем указатель для доступа к методу
    }

    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    GraphicsEngine::clear();

    if (player) {
        player->draw_bullets();
        player->draw(); // Используем указатель для доступа к методу
    }

    GraphicsEngine::drawAll(); // Render all shapes
}

// free game data in this function
void finalize()
{
    delete player; // Освобождаем память, выделенную для игрока
    player = nullptr; // Устанавливаем указатель в nullptr для безопасности
}

