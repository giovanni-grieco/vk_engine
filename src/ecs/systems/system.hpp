#pragma once

namespace engine{
    class System{
        public:
            virtual ~System() = default;
            virtual void start() = 0;
            virtual void update() = 0;
    };
}