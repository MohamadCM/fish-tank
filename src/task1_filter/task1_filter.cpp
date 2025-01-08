// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>

// Size of the framebuffer
const unsigned int SIZE = 512;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel
{
    unsigned char r, g, b; // Each color is a byte
};

int main()
{
    // Initialize a framebuffer
    auto framebuffer = new Pixel[SIZE][SIZE];

    std::ifstream inputFile("lena.raw", std::ios::binary);
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open lena.raw" << std::endl;
        delete[] framebuffer;
        return EXIT_FAILURE;
    }

    inputFile.read(reinterpret_cast<char*>(framebuffer), SIZE * SIZE * sizeof(Pixel));
    inputFile.close();

    // Traverse the framebuffer
    for (unsigned int y = 0; y < SIZE; y++)
    {
        for (unsigned int x = 0; x < SIZE; x++)
        {
            // Inverting colors
            framebuffer[y][x].r = 255 - framebuffer[y][x].r;
            framebuffer[y][x].g = 255 - framebuffer[y][x].g;
            framebuffer[y][x].b = 255 - framebuffer[y][x].b;
        }
    }

    std::ofstream outputFile("result.raw", std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Error: Unable to open result.raw" << std::endl;
        delete[] framebuffer;
        return EXIT_FAILURE;
    }

    std::cout << "Generating result.raw file ..." << std::endl;

    // TODO: Write the framebuffer to the file and close it
    outputFile.write(reinterpret_cast<const char*>(framebuffer), SIZE * SIZE * sizeof(Pixel));
    outputFile.close();

    std::cout << "Done." << std::endl;
    delete[] framebuffer;
    return EXIT_SUCCESS;
}
