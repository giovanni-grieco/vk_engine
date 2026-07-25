#include "ecs/components/component_array.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"

#include <memory>

using namespace engine;

void setup(EntityManager& em, ComponentManager& cm)
{
    auto e = em.createEntity();

    Transform t {{1.5f, 1.5f, 0.f},{}};

    cm.registerComponent<Transform>();
    cm.addComponent<Transform>(e, t);
}

int main()
{   
    std::cout<<"vk_engine started!\n" << "-----------------\n";
    Entity e1 = 1;
    ComponentManager& cm = ComponentManager::getInstance();
    
    ComponentManager& cm1 = cm;

    ComponentManager::getInstance().registerComponent<Transform>();

    EntityManager& em = EntityManager::getInstance();

    EntityManager::getInstance();
    std::cout<< "-----------------\n"<<"vk_engine closing!\n";
}