#include "system_manager.hpp"

#include <algorithm>
#include "systems/transform_system.hpp"

namespace engine
{
    std::unique_ptr<SystemManager> SystemManager::instance = nullptr;

    SystemManager &SystemManager::getInstance()
    {
        if (instance == nullptr)
        {
            instance = std::unique_ptr<SystemManager>(std::make_unique<SystemManager>());
        }
        return *instance;
    }

    SystemManager::SystemManager(){
        this->transformSystem = std::make_unique<TransformSystem>();
    }

    void SystemManager::start()
    {
        this->transformSystem->start();
        for (auto &system : systems)
        {
            system->start();
        }
        this->transformSystem->update();
    }
    void SystemManager::update()
    {
        for (auto &system : systems)
        {
            system->update();
        }
        this->transformSystem->update();
    }

    void SystemManager::registerSystem(std::unique_ptr<System> system)
    {
        systems.push_back(std::move(system));
    }

}