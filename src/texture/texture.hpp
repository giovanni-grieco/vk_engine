#pragma once

#include <string>
#include <vector>

namespace engine{
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