#pragma once

#include "system.hpp"

namespace engine{
    class Translator : public System{
        void start() override;
        void update() override;
    };
}