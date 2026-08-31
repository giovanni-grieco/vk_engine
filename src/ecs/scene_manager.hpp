#pragma once

#include "ecs/entity/entity.hpp"

#include <memory>

namespace engine
{
    // Stateless facade over EntityManager + ComponentManager.
    //
    // It is the single owner of the parent <-> children invariant: game code
    // creates, destroys and relates entities through here instead of touching
    // the managers directly. It holds no data of its own.
    class SceneManager
    {
    public:
        static SceneManager &getInstance();
        SceneManager() = default;

        Entity createEntity();
        void destroyEntity(Entity entity);

        void parent(Entity child, Entity newParent);
        void unparent(Entity child);

    private:

        static std::unique_ptr<SceneManager> instance;

        SceneManager(const SceneManager &) = delete;
        SceneManager &operator=(const SceneManager &) = delete;
    };
}
