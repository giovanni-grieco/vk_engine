#pragma once

#include "ecs/systems/system.hpp"

#include <memory>
#include <vector>
#include <iostream>

namespace engine
{
    class SystemManager
    {
    private:

        // Prevent copying — unique_ptr members are non-copyable
        SystemManager(const SystemManager&) = delete;
        SystemManager& operator=(const SystemManager&) = delete;    

        static std::unique_ptr<SystemManager> instance;
        std::vector<std::unique_ptr<System>> systems;

    public:
        static SystemManager& getInstance()
        {
            if (instance == nullptr)
            {
                instance = std::unique_ptr<SystemManager>(std::make_unique<SystemManager>());
            }
            return *instance;
        }

        SystemManager() = default;

        void start();
        void update();

        void registerSystem(std::unique_ptr<System> system);
    };
}