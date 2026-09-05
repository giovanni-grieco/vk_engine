#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.hpp"

#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace engine
{


    void createTextureFromFile(Texture& result, std::string filepath){
        stbi_set_flip_vertically_on_load(true); // Vulkan's image origin is the top-left corner
        int texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load(filepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image \"" + filepath + "\"!");
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
            throw std::runtime_error("failed to load texture image \"" + filepath + "\"!");
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

    Texture createBrightGreenLaserTexture(int width, int height)
    {
        Texture texture{};
        texture.texWidth = width;
        texture.texHeight = height;
        texture.texChannels = 4;
        texture.pixels.resize(static_cast<size_t>(width) * height * 4);

        const float cx = static_cast<float>(width - 1) * 0.5f;
        const float cy = static_cast<float>(height - 1) * 0.5f;
        const float maxDist = std::sqrt(cx * cx + cy * cy);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const float dx = static_cast<float>(x) - cx;
                const float dy = static_cast<float>(y) - cy;
                const float dist = std::sqrt(dx * dx + dy * dy) / maxDist; // 0 at center, 1 at corner

                // White-hot core in the middle, fading into a bright green glow,
                // then down to black toward the edges.
                const float core = std::max(0.0f, 1.0f - dist * 2.5f);
                const float glow = std::max(0.0f, 1.0f - dist);

                const float r = 255.0f * (glow * 0.10f + core);
                const float g = 255.0f * (glow * 1.00f + core);
                const float b = 255.0f * (glow * 0.10f + core);

                const size_t idx = (static_cast<size_t>(y) * width + x) * 4;
                texture.pixels[idx + 0] = static_cast<unsigned char>(std::min(r, 255.0f));
                texture.pixels[idx + 1] = static_cast<unsigned char>(std::min(g, 255.0f));
                texture.pixels[idx + 2] = static_cast<unsigned char>(std::min(b, 255.0f));
                texture.pixels[idx + 3] = 255; // opaque; the shader samples RGB only
            }
        }

        return texture;
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