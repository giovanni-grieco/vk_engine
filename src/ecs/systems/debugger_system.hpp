#pragma once

#include "system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"
#include <iostream>

namespace engine{
    class Debugger : public System{
        public:
            Debugger(bool enableDebugger){
                flag=enableDebugger;
            }

            void start() override {
                if(flag)
                    std::cout << "Debugger start() called\n";
            }   

            void update() override {
                if (flag){
                    std::cout << "Debugger update() called\n";
                    for (Transform& transform : ComponentManager::getInstance().getComponentArray<Transform>().getComponents()){
                        transform.dump();
                    }
                }
            }
        private:
            bool flag=true;
    };
}