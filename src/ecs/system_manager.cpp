#include "system_manager.hpp"

#include <algorithm>

namespace engine{
    std::unique_ptr<SystemManager> SystemManager::instance = nullptr;

    void SystemManager::start(){
        for(auto& system: systems){
            system->start();
        }
    }
    void SystemManager::update(){
        for(auto& system: systems){
            system->update();
        }
    }

    void SystemManager::registerSystem(std::unique_ptr<System> system){
        systems.push_back(std::move(system));
    }

}