#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <vector>
#include <tuple>
#include "Engine.h"

#include "GraphicsEngine.h"
#include <iostream>

#include <algorithm>
#include <vector>
#include <tuple>
#include <utility> // Äëÿ std::pair

uint32_t* GraphicsEngine::draw_buffer = nullptr;

vec2d Camera::position = vec2d(100, 100);
float Camera::speed = 10.0f;

void Camera::move(vec2d new_position, float delta) {
    // Calculate the target position based on the new desired position
    vec2d target_position = new_position;

    // Calculate the smoothing factor (how quickly the camera moves towards the target)
    float smooth_factor = speed * delta; // speed is in units per second

    // Use linear interpolation to smoothly move the camera
    position = vec2d::lerp(position, target_position, smooth_factor);
}

// Function to create a color from RGB values
uint32_t GraphicsEngine::create_color(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

// Clear the backbuffer
void GraphicsEngine::clear() {
    memset(draw_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
}

void GraphicsEngine::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        Draw(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int err2 = err * 2;
        if (err2 > -dy) { err -= dy; x1 += sx; }
        if (err2 < dx) { err += dx; y1 += sy; }
    }
};

void GraphicsEngine::drawRectangle(int x, int y, int width, int height, uint32_t color) {
    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            Draw(i, j, color);
        }
    }
}


void GraphicsEngine::Draw(int x, int y, uint32_t color) {
    x -= Camera::position.x;
    y -= Camera::position.y;
    if (x >= 0 && x < SCREEN_WIDTH - 1 && y >= 0 && y < SCREEN_HEIGHT - 1)
        buffer[y][x] = color;
}

void GraphicsEngine::DrawWOffset(int x, int y, float offset, uint32_t color) {
    // Determine which layer you are drawing for and adjust accordingly
    // Calculate adjusted positions based on layer speed
    int adjustedX = static_cast<int>(x - Camera::position.x * offset);
    int adjustedY = static_cast<int>(y - Camera::position.y * offset);

    // Draw only if within bounds
    if (adjustedX >= 0 && adjustedX < SCREEN_WIDTH - 1 && adjustedY >= 0 && adjustedY < SCREEN_HEIGHT - 1) {
        buffer[adjustedY][adjustedX] = color; // Use layer color or texture
    }
}

void GraphicsEngine::drawPolygon(const polygon& poly, uint32_t color) {

    int n = poly.p.size();
    for (int i = 0; i < poly.p.size(); i++)
        drawLine(poly.p[i].x, poly.p[i].y, poly.p[(i + 1) % poly.p.size()].x, poly.p[(i + 1) % poly.p.size()].y, color);
    
    //// Loop through each edge of the polygon
    //for (int i = 0; i < n; i++) {
    //    vec2d v1 = poly.p[i];
    //    vec2d v2 = poly.p[(i + 1) % n];
    //
    //    // Ensure v1 is the lower point
    //    if (v1.y > v2.y) std::swap(v1, v2);
    //
    //    // Scanline fill between v1 and v2
    //    for (int y = static_cast<int>(v1.y); y <= static_cast<int>(v2.y); y++) {
    //        if (y < 0 || y >= SCREEN_HEIGHT) continue; // Skip out of bounds
    //
    //        // Calculate intersection points with the edge
    //        float x_left = (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y) + v1.x;
    //        float x_right = (y - v1.y) * (v2.x - v1.x) / (v2.y - v1.y) + v1.x;
    //
    //        // Draw horizontal line between x_left and x_right
    //        int x_start = static_cast<int>(std::floor(x_left));
    //        int x_end = static_cast<int>(std::floor(x_right));
    //
    //        for (int x = x_start; x <= x_end; x++) {
    //            if (x >= 0 && x < SCREEN_WIDTH) {
    //                Draw(x, y, color);
    //            }
    //        }
    //    }
    //}
}

void GraphicsEngine::drawCircle(int centerX, int centerY, int radius, uint32_t color) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                int px = centerX + x;
                int py = centerY + y;
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                    Draw(px, py, color);
                }
            }
        }
    }
}