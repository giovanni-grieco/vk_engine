#pragma once

#include "ecs/systems/system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"

#include <memory>
#include <vector>
#include <iostream>

namespace engine
{
    class SystemManager
    {

    public:
        static SystemManager &getInstance();

        SystemManager() = default;

        void start();
        void update();

        void registerSystem(std::unique_ptr<System> system);

    private:
        static std::unique_ptr<SystemManager> instance;
        
        // Prevent copying — unique_ptr members are non-copyable
        SystemManager(const SystemManager &) = delete;
        SystemManager &operator=(const SystemManager &) = delete;
        std::vector<std::unique_ptr<System>> systems;
    };
}