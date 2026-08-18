#include "car_system.hpp"

namespace engine{

    void CarSystem::start() {

    }

    void CarSystem::update() {
        ComponentManager& cm = ComponentManager::getInstance(); 
        for(const Entity e : cm.getEntitiesWithComponent<CarComponent>()){
            std::cout<<"Entity "<<e<<" is a car!\n";
        }
    }
}
