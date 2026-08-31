#include "scene_manager.hpp"

#include "ecs/entity_manager.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/parent.hpp"
#include "ecs/components/children.hpp"

#include <algorithm>
#include <iostream>

namespace engine
{
    std::unique_ptr<SceneManager> SceneManager::instance = nullptr;

    SceneManager &SceneManager::getInstance()
    {
        if (instance == nullptr)
        {
            instance = std::unique_ptr<SceneManager>(std::make_unique<SceneManager>());
        }
        return *instance;
    }

    Entity SceneManager::createEntity()
    {
        return EntityManager::getInstance().createEntity();
    }

    void SceneManager::destroyEntity(Entity entity)
    {
        EntityManager &em = EntityManager::getInstance();
        ComponentManager &cm = ComponentManager::getInstance();

        if (!em.exists(entity))
        {
            std::cerr << "SceneManager::destroyEntity: entity " << entity << " does not exist.\n";
            return;
        }

        // 1. Detach from own parent.
        if (cm.hasComponent<ParentComponent>(entity))
        {
            Entity p = cm.getComponent<ParentComponent>(entity).parent;
            if (p != NULL_ENTITY && cm.hasComponent<ChildrenComponent>(p))
            {
                auto &kids = cm.getComponent<ChildrenComponent>(p).children;
                kids.erase(std::remove(kids.begin(), kids.end(), entity), kids.end());
            }
        }

        // 2. Recursively destroy children (copy: recursion mutates the vector).
        if (cm.hasComponent<ChildrenComponent>(entity))
        {
            auto kids = cm.getComponent<ChildrenComponent>(entity).children;
            for (Entity child : kids)
            {
                destroyEntity(child);
            }
        }

        // 3. Destroy the entity itself (also strips its remaining components).
        em.destroyEntity(entity, cm);
    }

    void SceneManager::parent(Entity child, Entity newParent)
    {
        EntityManager &em = EntityManager::getInstance();
        ComponentManager &cm = ComponentManager::getInstance();

        if (!em.exists(child))
        {
            std::cerr << "SceneManager::parent: child entity " << child << " does not exist.\n";
            return;
        }

        if (newParent == NULL_ENTITY)
        {
            unparent(child);
            return;
        }

        if (!em.exists(newParent))
        {
            std::cerr << "SceneManager::parent: parent entity " << newParent << " does not exist.\n";
            return;
        }

        // Ensure the relationship components exist (registered in init()).
        if (!cm.hasComponent<ParentComponent>(child))
            cm.addComponent<ParentComponent>(child, ParentComponent{});
        if (!cm.hasComponent<ChildrenComponent>(newParent))
            cm.addComponent<ChildrenComponent>(newParent, ChildrenComponent{});

        unparent(child);

        cm.getComponent<ParentComponent>(child).parent = newParent;
        cm.getComponent<ChildrenComponent>(newParent).children.push_back(child);
    }

    void SceneManager::unparent(Entity child)
    {
        ComponentManager &cm = ComponentManager::getInstance();

        if (!cm.hasComponent<ParentComponent>(child))
            return;

        Entity old = cm.getComponent<ParentComponent>(child).parent;
        if (old != NULL_ENTITY && cm.hasComponent<ChildrenComponent>(old))
        {
            auto &kids = cm.getComponent<ChildrenComponent>(old).children;
            kids.erase(std::remove(kids.begin(), kids.end(), child), kids.end());
        }
        cm.getComponent<ParentComponent>(child).parent = NULL_ENTITY;
    }
}
