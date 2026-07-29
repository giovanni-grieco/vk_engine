#include <gtest/gtest.h>
#include "ecs/entity_manager.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/car.hpp"

using namespace engine;

TEST(EntityRecycleTest, IDsAreRecycled) {
    EntityManager em;

    // Create two entities
    Entity a = em.createEntity();
    Entity b = em.createEntity();

    // They should have distinct IDs
    EXPECT_NE(a, b);

    // Destroy both
    em.destroyEntity(a);
    em.destroyEntity(b);

    // Create new entities — they should recycle the old IDs (LIFO order)
    Entity c = em.createEntity();
    EXPECT_EQ(c, b) << "First recycled entity should get the most recently destroyed ID";

    Entity d = em.createEntity();
    EXPECT_EQ(d, a) << "Second recycled entity should get the older destroyed ID";

    // Clean up
    em.destroyEntity(c);
    em.destroyEntity(d);
}

TEST(EntityRecycleTest, RepeatedRecyclingCycle) {
    EntityManager em;
    bool first = true;
    Entity old;
    // Do several create/destroy cycles and verify IDs stay bounded
    for (int i = 0; i < 100; ++i) {
        Entity e = em.createEntity();
        ASSERT_LT(e, MAX_ENTITIES) << "Entity ID must stay within MAX_ENTITIES";
        if(!first){
            EXPECT_EQ(e, old);
        }
        em.destroyEntity(e);
        if(first){
            old = e;
            first=false;
        };
    }
    // No crash = pass
    SUCCEED();
}

TEST(EntityRecycleTest, DeletionWithComponents) {
    EntityManager em;
    ComponentManager cm;
    
    cm.registerComponent<Transform>();
    cm.registerComponent<Car>();

    Entity e1 = em.createEntity();
    Entity e2 = em.createEntity();

    Transform t1,t2;
    Car c1,c2;

    cm.addComponent<Transform>(e1, t1);
    cm.addComponent<Car>(e1, c1);

    cm.addComponent<Transform>(e2, t2);
    cm.addComponent<Car>(e2, c2);

    EXPECT_TRUE(2 == cm.getComponentArray<Transform>().getComponents().size());

    SUCCEED();
}