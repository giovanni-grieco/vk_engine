#pragma once

#include "ecs/systems/system.hpp"
#include "ecs/component_manager.hpp"

#include <memory>
#include <vector>

namespace engine
{
    class SystemManager
    {

    public:
        static SystemManager &getInstance();

        SystemManager() = default;
        // Prevent copying — unique_ptr members are non-copyable
        SystemManager(const SystemManager &) = delete;
        SystemManager &operator=(const SystemManager &) = delete;

        void start();
        void update();

        void registerSystem(std::unique_ptr<System> system);
        
    private:
        static std::unique_ptr<SystemManager> instance;
        std::vector<std::unique_ptr<System>> systems;
    };
}