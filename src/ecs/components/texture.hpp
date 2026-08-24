#pragma once

#include "texture/texture.hpp"

namespace engine{
    struct TextureComponent{
        Texture texture;
        TextureID textureHandle = -1;
    };
}