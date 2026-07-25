#pragma once

#include "system.hpp"

namespace engine{
    class Translator : System{
        void start() override;
        void update() override;
    };
}