#include "transform_system.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/parent.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/entity/entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace engine{
    
    void TransformSystem::start() {

    }

    bool hasParent(Entity e, ComponentManager& cm, EntityManager& em){
         bool hasParent = cm.hasComponent<ParentComponent>(e) && //se ha un genitore
                cm.getComponent<ParentComponent>(e).parent != NULL_ENTITY && //se il genitore è inizializzato
                em.exists(cm.getComponent<ParentComponent>(e).parent); // se il genitore effettivamente esiste
        return hasParent;
    }

    Entity parentOf(Entity e, ComponentManager& cm){
        Entity parent = cm.getComponent<ParentComponent>(e).parent;
        return parent;
    }

    glm::mat4 makeLocalMatrix(LocalTransformComponent transform){
        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position)
                            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
                            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
                            * glm::scale(glm::mat4(1.0f), transform.scale);
        return model;
    }

    void TransformSystem::update() {
        auto& cm = ComponentManager::getInstance();
        auto& em = EntityManager::getInstance();

        std::vector<Entity> stack;
        for(Entity e : cm.getEntitiesWithComponent<LocalTransformComponent>()){
            if(!hasParent(e, cm, em)){
                stack.push_back(e);
            }
        }

        while(!stack.empty()){
            Entity e = stack.back();
            stack.pop_back();

            LocalTransformComponent transfrom = cm.getComponent<LocalTransformComponent>(e);
            glm::mat4 localMatrix = makeLocalMatrix(transfrom);
            
            if(hasParent(e, cm, em)){
                cm.getComponent<WorldTransformComponent>(e).matrix = cm.getComponent<WorldTransformComponent>(parentOf(e, cm)).matrix * localMatrix;
            }else{
                cm.getComponent<WorldTransformComponent>(e).matrix = localMatrix;
            }

            if(cm.hasComponent<ChildrenComponent>(e)){
                for(Entity c: cm.getComponent<ChildrenComponent>(e).children){
                    stack.push_back(c);
                }
            }
        }
    }
}