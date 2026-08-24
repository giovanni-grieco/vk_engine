#pragma once

#include <string>
#include <vector>
#include <stdint.h>

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

}