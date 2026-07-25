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
    EntityManager em {};
    ComponentManager cm {};

    setup(em, cm);

    auto entity = em.getActiveEntities().at(0);

    cm.getComponent<Transform>(entity).dump();

}