#include "ecs/component_manager.hpp"

namespace engine
{
    std::unique_ptr<ComponentManager> ComponentManager::instance = nullptr;

    ComponentManager &ComponentManager::getInstance()
    {
        if (instance == nullptr)
        {
            instance = std::unique_ptr<ComponentManager>(std::make_unique<ComponentManager>());
        }
        return *instance;
    }
}
