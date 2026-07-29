#include "ecs/components/component_array.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system_manager.hpp"
#include "ecs/systems/translator_system.hpp"
#include "ecs/systems/debugger_system.hpp"

#include <memory>

using namespace engine;

int main()
{   
    std::cout<<"vk_engine started!\n" << "-----------------\n";

    EntityManager& entityManager = EntityManager::getInstance();
    ComponentManager& componentManager = ComponentManager::getInstance();
    SystemManager& systemManager = SystemManager::getInstance();

    componentManager.registerComponent<Transform>();

    systemManager.registerSystem(std::make_unique<Debugger>(true));
    systemManager.registerSystem(std::make_unique<Translator>(0.143f));

    Entity e = entityManager.createEntity();
    
    Transform t {{50.f, 50.f, 50.f}, {0.f, 0.f, 0.f}};

    componentManager.addComponent<Transform>(e, t);

    systemManager.start();

    for(int i = 0; i<5; i++){
        systemManager.update();
    }

    t.dump();

    std::cout<< "-----------------\n"<<"vk_engine closing!\n";
}