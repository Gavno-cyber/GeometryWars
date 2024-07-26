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


// Function to create a color from RGB values
uint32_t GraphicsEngine::create_color(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

// Clear the backbuffer
void GraphicsEngine::clear() {
    memset(draw_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    shapes.clear();
}

void GraphicsEngine::drawPolygon(const std::vector<std::pair<int, int>>& vertices, uint32_t color) {
    shapes.emplace_back('P', 0, 0, 0, 0, vertices, color);
}
// Draw a filled rectangle and store it
void GraphicsEngine::drawRectangle(int x, int y, int width, int height, uint32_t color) {
    shapes.emplace_back('R', x, y, width, height, std::vector<std::pair<int, int>>(), color);
}

// Draw a filled circle and store it
void GraphicsEngine::drawCircle(int centerX, int centerY, int radius, uint32_t color) {
    shapes.emplace_back('C', centerX, centerY, radius, 0, std::vector<std::pair<int, int>>(), color);
}

// Draw a line and store it
void GraphicsEngine::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    shapes.emplace_back('L', x1, y1, x2, y2, std::vector<std::pair<int, int>>(), color);
}



// Draw all shapes stored in the vector
void GraphicsEngine::drawAll() {
    for (const auto& shape : shapes) {
        char type = std::get<0>(shape);
        if (type == 'R') {
            drawRectangleInternal(std::get<1>(shape), std::get<2>(shape), std::get<3>(shape), std::get<4>(shape), std::get<6>(shape));
        }
        else if (type == 'C') {
            drawCircleInternal(std::get<1>(shape), std::get<2>(shape), std::get<3>(shape), std::get<6>(shape));
        }
        else if (type == 'L') {
            drawLineInternal(std::get<1>(shape), std::get<2>(shape), std::get<3>(shape), std::get<4>(shape), std::get<6>(shape));
        }
        else if (type == 'P') {
            drawPolygonInternal(std::get<5>(shape), std::get<6>(shape));
        }
    }
}

void GraphicsEngine::drawLineInternal(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < SCREEN_WIDTH && y1 >= 0 && y1 < SCREEN_HEIGHT) {
            draw_buffer[y1 * SCREEN_WIDTH + x1] = color;
        }
        if (x1 == x2 && y1 == y2) break;
        int err2 = err * 2;
        if (err2 > -dy) { err -= dy; x1 += sx; }
        if (err2 < dx) { err += dx; y1 += sy; }
    }
};

uint32_t* GraphicsEngine::draw_buffer = *buffer;
std::vector<std::tuple<char, int, int, int, int, std::vector<std::pair<int, int>>, uint32_t>> GraphicsEngine::shapes;

void GraphicsEngine::drawRectangleInternal(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (x + i >= 0 && x + i < SCREEN_WIDTH && y + j >= 0 && y + j < SCREEN_HEIGHT) {
                draw_buffer[(y + j) * SCREEN_WIDTH + (x + i)] = color;
            }
        }
    }
}

void GraphicsEngine::drawPolygonInternal(const std::vector<std::pair<int, int>>& vertices, uint32_t color) {
    int numVertices = vertices.size();
    std::vector<int> edges[SCREEN_HEIGHT];

    // Create edge table
    for (int i = 0; i < numVertices; ++i) {
        int x1 = vertices[i].first;
        int y1 = vertices[i].second;
        int x2 = vertices[(i + 1) % numVertices].first;
        int y2 = vertices[(i + 1) % numVertices].second;

        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        if (y1 == y2) continue; // Ignore horizontal edges

        // Calculate slope
        float slope = static_cast<float>(x2 - x1) / (y2 - y1);

        // Add edge to edge table
        for (int y = y1; y < y2; ++y) {
            int x = static_cast<int>(x1 + slope * (y - y1));
            edges[y].push_back(x);
        }
    }

    // Fill the polygon
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        if (edges[y].empty()) continue;

        // Sort x-coordinates
        std::sort(edges[y].begin(), edges[y].end());

        // Fill between pairs of edges
        for (size_t i = 0; i < edges[y].size(); i += 2) {
            if (i + 1 < edges[y].size()) {
                int xStart = edges[y][i];
                int xEnd = edges[y][i + 1];

                for (int x = xStart; x <= xEnd; ++x) {
                    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                        draw_buffer[y * SCREEN_WIDTH + x] = color;
                    }
                }
            }
        }
    }
}

void GraphicsEngine::drawCircleInternal(int centerX, int centerY, int radius, uint32_t color) {
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                int px = centerX + x;
                int py = centerY + y;
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                    draw_buffer[py * SCREEN_WIDTH + px] = color;
                }
            }
        }
    }
}

