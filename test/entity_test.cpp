#include <gtest/gtest.h>
#include "ecs/entity_manager.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/car.hpp"

using namespace engine;

TEST(EntityTest, CreateAndDestroy) {
    EntityManager em;
    ComponentManager cm;

    cm.registerComponent<Transform>();

    Entity e = em.createEntity();

    Transform t{{1.f, 2.f, 3.f}, {0.f, 0.f, 0.f}};
    cm.addComponent<Transform>(e, t);

    EXPECT_TRUE(cm.hasComponent<Transform>(e));

    em.destroyEntity(e, cm);

    EXPECT_FALSE(cm.hasComponent<Transform>(e));
}