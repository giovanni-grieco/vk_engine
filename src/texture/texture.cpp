#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.hpp"

#include <stdexcept>
#include <iostream>

namespace engine
{


    void createTextureFromFile(Texture& result, std::string filepath){
        stbi_set_flip_vertically_on_load(true); // Vulkan's image origin is the top-left corner
        int texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }

        size_t totalSize = texWidth * texHeight * 4;
        result.pixels.reserve(totalSize);
        for (int i = 0; i < totalSize; i++)
        {
            result.pixels.push_back(static_cast<unsigned char>(pixels[i]));
        }

        result.texWidth = texWidth;
        result.texHeight = texHeight;
        result.texChannels = texChannels;

        stbi_image_free(pixels);
    }

    Texture createTextureFromFile(std::string filepath)
    {
        Texture result{};

        stbi_set_flip_vertically_on_load(true); // Vulkan's image origin is the top-left corner
        int texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }

        size_t totalSize = texWidth * texHeight * 4;
        result.pixels.reserve(totalSize);
        for (int i = 0; i < totalSize; i++)
        {
            result.pixels.push_back(static_cast<unsigned char>(pixels[i]));
        }

        result.texWidth = texWidth;
        result.texHeight = texHeight;
        result.texChannels = texChannels;

        stbi_image_free(pixels);

        return result;
    }

    int Texture::size()
    {
        return texWidth * texHeight * 4;
    }

    void Texture::dump()
    {
        std::cout << "Texture dump:\n\t"
                  << "texWidth: " << texWidth << "\n\t"
                  << "texHeight: " << texHeight << "\n\t"
                  << "texChannels: " << texChannels << "\n\t"
                  << "vector size: " << pixels.size() << "\n\t"
                  << "calculated size: " << size() << "\n\t";
    }
}