// Task 3 - Implement Bresenham drawing alg.
//        - Draw a star using lines
//        - Make use of std::vector to define the shape
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <ppgso/ppgso.h>

// Size of the framebuffer
const unsigned int SIZE = 512;

struct Point
{
    int x, y;
};

// Bresenham drawing algorithm
void drawLine(ppgso::Image& framebuffer, Point& from, Point& to) {
    int dx = std::abs(to.x - from.x);
    int dy = std::abs(to.y - from.y);
    int sx = (from.x < to.x) ? 1 : -1;
    int sy = (from.y < to.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        framebuffer.setPixel(from.x, from.y, 255, 255, 255); // Set pixel to white (RGB 255, 255, 255)

        if (from.x == to.x && from.y == to.y) break;

        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            from.x += sx;
        }
        if (e2 < dx) {
            err += dx;
            from.y += sy;
        }
    }
}

int main()
{
    // Use ppgso::Image as our framebuffer
    ppgso::Image framebuffer(SIZE, SIZE);

    // Generate star points
    std::vector<Point> points;
    int centerX = SIZE / 2;
    int centerY = SIZE / 2;
    int radius = 200;
    int numPoints = 5;

    for (int i = 0; i < numPoints; i++)
    {
        double angle = i * 4 * M_PI / numPoints;
        int x = centerX + static_cast<int>(radius * cos(angle));
        int y = centerY + static_cast<int>(radius * sin(angle));
        points.push_back({x, y});
        std::cout << x << "," << y << "\n" << std::endl;
    }

    // Draw lines
    for (unsigned int i = 0; i < points.size() - 1; i++)
        drawLine(framebuffer, points[i], points[(i + 1) % points.size()]);


    // Note: this line works incorrectly
    // drawLine(framebuffer, points[4], points[0]);

    // Save the result
    std::cout << "Generating task2_bresenham.bmp file ..." << std::endl;
    ppgso::image::saveBMP(framebuffer, "task2_bresenham.bmp");

    std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}
