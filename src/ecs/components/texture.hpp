#pragma once

#include "texture/texture.hpp"

#include <vector>

namespace engine{
    struct TextureComponent{
        // One texture handle per material group, index-aligned with the mesh's
        // submeshes. Empty means "use the default texture for every submesh".
        std::vector<TextureID> textureHandles;
    };
}