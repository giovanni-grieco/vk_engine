#include "ecs/components/transform.hpp"
#include "ecs/components/car.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system_manager.hpp"
#include "ecs/systems/translator_system.hpp"
#include "ecs/systems/debugger_system.hpp"
#include "ecs/systems/car_system.hpp"

#include <memory>

using namespace engine;


int main()
{   
    std::cout<<"vk_engine started!\n" << "-----------------\n";

    EntityManager& entityManager = EntityManager::getInstance();
    ComponentManager& componentManager = ComponentManager::getInstance();
    SystemManager& systemManager = SystemManager::getInstance();

    componentManager.registerComponent<TransformComponent>();
    componentManager.registerComponent<CarComponent>();

    bool debuggerEnableFlag = true;

    float translatorSpeed = 0.1473;

    systemManager.registerSystem(std::make_unique<Debugger>(debuggerEnableFlag));
    systemManager.registerSystem(std::make_unique<Translator>(translatorSpeed));
    systemManager.registerSystem(std::make_unique<CarSystem>());

    Entity e = entityManager.createEntity();
    std::cout<<"e: "<<e<<"\n";
    Entity e1 = entityManager.createEntity();
    std::cout<<"e1: "<<e1<<"\n";
    TransformComponent t {{50.f, 50.f, 50.f}, {0.f, 0.f, 0.f}};

    componentManager.addComponent<TransformComponent>(e, t);

    systemManager.start();

    for(int i = 0; i<5; i++){
        systemManager.update();
    }

    entityManager.destroyEntity(e);
    Entity e2 = entityManager.createEntity();
    CarComponent car {"Ford", "Mustang", 1977};
    componentManager.addComponent<CarComponent>(e2, car);
    std::cout<<"e2: "<<e2<<"\n";

    

    for(int i = 0; i<5; i++){
        systemManager.update();
    }



    std::cout<< "-----------------\n"<<"vk_engine closing!\n";
}