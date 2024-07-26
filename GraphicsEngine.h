#pragma once

#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <stdint.h>
#include <vector>
#include <tuple>

class GraphicsEngine {
public:
    static uint32_t* draw_buffer;  // Backbuffer pointer

    // Function to create a color from RGB values
    static uint32_t create_color(uint8_t r, uint8_t g, uint8_t b);

    // Clear the backbuffer
    static void clear();

    // Draw a filled rectangle and store it
    static void drawRectangle(int x, int y, int width, int height, uint32_t color);

    static void drawPolygon(const std::vector<std::pair<int, int>>& vertices, uint32_t color);

    // Draw a filled circle and store it
    static void drawCircle(int centerX, int centerY, int radius, uint32_t color);

    // Draw a line and store it
    static void drawLine(int x1, int y1, int x2, int y2, uint32_t color);

    // Draw all shapes stored in the vector
    static void drawAll();

private:


    static std::vector<std::tuple<char, int, int, int, int, std::vector<std::pair<int, int>>, uint32_t>> shapes;  // Vector to store shapes

    // Private methods for internal drawing operations
    static void drawRectangleInternal(int x, int y, int width, int height, uint32_t color);
    static void drawCircleInternal(int centerX, int centerY, int radius, uint32_t color);
    static void drawLineInternal(int x1, int y1, int x2, int y2, uint32_t color);
    static void drawPolygonInternal(const std::vector<std::pair<int, int>>& vertices, uint32_t color);
};

#endif // GRAPHICS_ENGINE_H