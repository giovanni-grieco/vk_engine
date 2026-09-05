#pragma once

#include <string>
#include <vector>

namespace engine{

    typedef int32_t TextureID;

    struct Texture{
        int texWidth = 0;
        int texHeight = 0;
        int texChannels = 0;

        std::vector<unsigned char> pixels;

        int size();
        void dump();
    };

    Texture createTextureFromFile(std::string filepath);
    void createTextureFromFile(Texture& result, std::string filepath);

    // Procedurally generates a bright-green laser bolt texture: a white-hot core
    // that fades into a green glow and then to black at the edges. No file needed.
    Texture createBrightGreenLaserTexture(int width = 64, int height = 64);

}