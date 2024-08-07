#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include "GraphicsEngine.h"
#include "PhysicsEngine.h"
#include "GameObjects.h"
#include <cstdlib>
#include <ctime>
#include <windows.h>

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
EnemySpawner* enemySpawner = nullptr;

vec2d arrayStars[10000];
float arrayLayers[10000];

struct sBullet
{
    uint32_t color;
    vec2d pos;
    vec2d vel;
    bool remove = false;
};

std::list<sBullet> listFragments;

std::vector<polygon> walls;

int score = 0;
int kill_em_all = 5;

float fPowerUpCooldownTime = 0.0f; // Time remaining until the power-up can be used again
float fPowerUpCooldownDuration = 1.0f; // Duration of the cooldown period (in seconds)

// initialize game data in this function
void initialize()
{
    //text = "Здоровье игрока: %d";
    for (auto& s : arrayStars) {
        s = { 5000 - (float)(rand() % SCREEN_WIDTH * 7), 5000 - (float)(rand() % SCREEN_HEIGHT * 7) };
    }

    for (auto& s : arrayLayers) {
        s = { 0.2f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
    }

    vec2d playerPosition(200, 100);

    walls = PolygonCollisions::createWalls(SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    //polygon s1 = PolygonCollisions::createWallSegment(vec2d(SCREEN_WIDTH / 2, SCREEN_HEIGHT + SCREEN_HEIGHT / 2), 0, SCREEN_WIDTH / 2);
    //
    //polygon s2 = PolygonCollisions::createQuad(vec2d(SCREEN_WIDTH / 2, SCREEN_HEIGHT + SCREEN_HEIGHT / 2), 0, SCREEN_WIDTH / 2);
    //
    //polygon s3 = PolygonCollisions::createQuad(vec2d(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 0, SCREEN_HEIGHT / 2);
    //
    //polygon s4 = PolygonCollisions::createQuad(vec2d(SCREEN_WIDTH / 2, -SCREEN_HEIGHT - SCREEN_HEIGHT / 2), 0, SCREEN_WIDTH / 2);
    //
    //walls.push_back(s1);
    //walls.push_back(s2);
    //walls.push_back(s3);
    //walls.push_back(s4);



    uint32_t playerColor = 0x00FFF0; // Пример цвета (зеленый)

    std::string playerName = "Hero"; // Инициализация playerName
    uint32_t width(50), height(50);

    polygon s = PolygonCollisions::createQuad(playerPosition, .0f, 30);

    std::unique_ptr<polygon> playerShape(make_unique<polygon>(s)); // Создание уникального указателя на Shape

    // Создание объекта Player
    player = new Player(playerName, std::move(playerShape), playerPosition, 700.0f, 5, playerColor);

    enemySpawner = new EnemySpawner(2.0f, *player);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (player) {
        player->update_state(dt);
        player->shoot(dt);// Используем указатель для доступа к методу


        // Update loop
        if (fPowerUpCooldownTime > 0.0f) {
            // Decrease the cooldown timer
            fPowerUpCooldownTime -= dt; // Assuming deltaTime is the time passed since the last frame
        }
        // Using the power-up
        if (is_mouse_button_pressed(1) && kill_em_all > 0 && fPowerUpCooldownTime <= 0.0f) {
            kill_em_all -= 1;
            fPowerUpCooldownTime = fPowerUpCooldownDuration; // Reset the cooldown timer
      
            for (const auto& e : enemySpawner->get_enemies()) {
                score += 150;
                for (int i = 0; i < 400; i++)
                {
                    // Generate random values for each color component (0-255)
                    int red = rand() % 256;   // Random value between 0 and 255
                    int green = rand() % 256; // Random value between 0 and 255
                    int blue = rand() % 256;  // Random value between 0 and 255

                    // Combine into a single color in the format 0xRRGGBB

                    uint32_t color = (red << 16) | (green << 8) | blue;

                    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
                    float speed = ((float)rand() / (float)RAND_MAX) * 400.0f + 50.0f;

                    listFragments.push_back({ color,
                        e->get_position(),
                        { cosf(angle) * speed, sinf(angle) * speed } });
                }
            }

            enemySpawner->remove_all_enemies();
        }

        // Update Player Bullets
        for (auto& b : player->listPlayerBullets)
        {
            // Position Bullet
            b.pos.x += (b.vel.x * dt);
            b.pos.y += (b.vel.y * dt);

            for (const auto& e : enemySpawner->get_enemies()) {
                
                if ((b.pos - (e->get_position())).magnitude() < 30)
                {
                    // Enemy has been hit
                    b.remove = true;
                    e->health -= 1.0f;

                    // Trigger explosion
                    if (e->health <= 0)
                    {
                        score += 150;
                        for (int i = 0; i < 300; i++)
                        {
                            // Generate random values for each color component (0-255)
                            int red = rand() % 150;   // Random value between 0 and 255
                            int green = rand() % 150; // Random value between 0 and 255
                            int blue = rand() % 150;  // Random value between 0 and 255

                            // Combine into a single color in the format 0xRRGGBB

                            uint32_t color = (red << 16) | (green << 8) | blue;

                            float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
                            float speed = ((float)rand() / (float)RAND_MAX) * 400.0f + 50.0f;
                            listFragments.push_back({color,
                                e->get_position(),
                                { cosf(angle) * speed, sinf(angle) * speed } });
                        }
                        break;
                    }
                }
            }
        }
        player->listPlayerBullets.remove_if([&](const Player::sBullet& b) {return b.pos.x<0 || b.pos.x>SCREEN_WIDTH || b.pos.y <0 || b.pos.y>SCREEN_HEIGHT || b.remove; });
        enemySpawner->remove_enemy();

        //не убивайте меня за это, я знаю, что можно было сделать красивый скейл статов через словарик
        if (score >= 1000) {
            player->speed = 800;
            player->number_of_bullets = 2;
            player->fPlayerGunReload = 0.08f;
            if (score >= 10000) {
                player->speed = 1000;
                player->number_of_bullets = 3;
                player->spread_angle = 22.0f;
                player->fPlayerGunReload = 0.07f;
                if (score >= 20000) {
                    player->speed = 1100;
                    player->fPlayerGunReload = 0.05f;
                    player->number_of_bullets = 4;
                    player->spread_angle = 22.0f;
                    if (score >= 50000) {
                        player->speed = 1200;
                        player->fPlayerGunReload = 0.03f;
                        player->number_of_bullets = 8;
                        player->spread_angle = 30.0f;
                        if (score >= 100000) {
                            player->speed = 1500;
                            player->fPlayerGunReload = 0.02f;
                            player->number_of_bullets = 10;
                            player->spread_angle = 35.0f;
                            if (score >= 200000) {
                                player->fPlayerGunReload = 0.01f;
                                player->number_of_bullets = 15;
                                player->spread_angle = 45.0f;
                            }
                        }
                    }
                }
            }
        }
    }

    // Update the enemy spawner
    enemySpawner->update(dt);

    // Update all enemies
    for (const auto& enemy : enemySpawner->get_enemies()) {
        enemy->update_state(dt);

        if (PolygonCollisions::ShapeOverlap_DIAGS(*player->shape, *enemy->shape)) {
            player->health -= 1;

            for (const auto& e : enemySpawner->get_enemies()) {
                for (int i = 0; i < 400; i++)
                {
                    // Generate random values for each color component (0-255)
                    int red = rand() % 256;   // Random value between 0 and 255
                    int green = rand() % 256; // Random value between 0 and 255
                    int blue = rand() % 256;  // Random value between 0 and 255

                    // Combine into a single color in the format 0xRRGGBB

                    uint32_t color = (red << 16) | (green << 8) | blue;

                    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
                    float speed = ((float)rand() / (float)RAND_MAX) * 400.0f + 50.0f;

                    listFragments.push_back({ color,
                        e->get_position(),
                        { cosf(angle) * speed, sinf(angle) * speed }});
                }
            }

            enemySpawner->remove_all_enemies();

            player->check_health();
            break;
        }
    }

    // Update Fragments
    for (auto& f : listFragments) f.pos = f.pos + f.vel * dt;

    // Remove finished fragments
    listFragments.remove_if([&](const sBullet& b) {return b.pos.x<0 || b.pos.x>SCREEN_WIDTH || b.pos.y <0 || b.pos.y>SCREEN_HEIGHT || b.remove; });

    for (auto& w : walls)
    {
        for (int i = 0; i < w.o.size(); i++)
            w.p[i] =
        {	// 2D Rotation Transform + 2D Translation
            (w.o[i].x * cosf(w.angle)) - (w.o[i].y * sinf(w.angle)) + w.pos.x,
            (w.o[i].x * sinf(w.angle)) + (w.o[i].y * cosf(w.angle)) + w.pos.y,
        };

        PolygonCollisions::ShapeOverlap_DIAGS_STATIC(*player->shape, w);
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

    // Draw Fragments
    for (auto& b : listFragments)
    {
        GraphicsEngine::drawLine(b.pos.x, b.pos.y, b.pos.x + b.vel.x / 15, b.pos.y + b.vel.y / 15, b.color);
    }

    //drawing border;
    GraphicsEngine::drawLine(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFFFFFF);
    GraphicsEngine::drawLine(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0xFFFFFF);
    GraphicsEngine::drawLine(SCREEN_WIDTH, 0, 0, 0, 0xFFFFFF);
    GraphicsEngine::drawLine(0, 0, 0, SCREEN_HEIGHT, 0xFFFFFF);

    //drawing stars;
    for (size_t i = 0; i < sizeof(arrayStars) / sizeof(arrayStars[0]); i++){
        auto& s = arrayStars[i];
        float offset = arrayLayers[i];
        GraphicsEngine::DrawWOffset(s.x, s.y, offset, 0xFFFFFF);
    }

    // Update all enemies
    for (const auto& enemy : enemySpawner->get_enemies()) {
        enemy->draw();
    }

    //updating player;
    if (player) {
        player->draw(); 
    }

    for (auto& b : player->listPlayerBullets) GraphicsEngine::drawCircle(b.pos.x, b.pos.y, 5, 0xF0FFFF);


    //Draw health
    for (int n = 0; n < player->health; ++n) {
        // Define rectangle dimensions
        int rectWidth = 30; // Width of each rectangle
        int rectHeight = 30; // Height of each rectangle
        int spacing = 10; // Space between rectangles

        // Calculate the starting position for the current rectangle
        int startX = 50 + n * (rectWidth + spacing); // X position
        int startY = 50; // Y position (fixed)

        // Fill the rectangle in the buffer
        for (int j = startY; j < startY + rectHeight; ++j) {
            for (int i = startX; i < startX + rectWidth; ++i) {

                buffer[j][i] = 0x0FF00F; // Set pixel color to green
            }
        }
    }

    // Generate random values for each color component (0-255)
    int red = rand() % 256;   // Random value between 0 and 255
    int green = rand() % 256; // Random value between 0 and 255
    int blue = rand() % 256;  // Random value between 0 and 255

    // Combine into a single color in the format 0xRRGGBB

    uint32_t color = (red << 16) | (green << 8) | blue;

    //Draw powerup
    for (int n = 0; n < kill_em_all; ++n) {
        // Define rectangle dimensions
        int rectWidth = 30; // Width of each rectangle
        int rectHeight = 30; // Height of each rectangle
        int spacing = 10; // Space between rectangles

        // Calculate the starting position for the current rectangle
        int startX = 50 + n * (rectWidth + spacing); // X position
        int startY = 100; // Y position (fixed)

        // Fill the rectangle in the buffer
        for (int j = startY; j < startY + rectHeight; ++j) {
            for (int i = startX; i < startX + rectWidth; ++i) {

                buffer[j][i] = color; // Set pixel color to green
            }
        }
    }

}

// free game data in this function
void finalize()
{
    delete player; 
    player = nullptr; 
}

