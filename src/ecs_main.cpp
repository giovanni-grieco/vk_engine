#include "components/component_array.hpp"
#include "components/transform.hpp"

using namespace engine;
using namespace std;

int main(){
    ComponentArray<Transform> ca {};

    Transform transform1 = {1.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f};
    Transform transform2 = {4.0f, 5.0f, 6.0f, 0.0f, 0.0f, 0.0f};
    Transform transform3 = {7.0f, 8.0f, 9.0f, 0.0f, 0.0f, 0.0f};
    Transform transform4 = {10.0f, 11.0f, 12.0f, 0.0f, 0.0f, 0.0f};

    Entity e1 = 1;
    Entity e2 = 2;
    Entity e3 = 3;
    Entity e4 = 4;

    ca.addComponent(e1, transform1);
    ca.addComponent(e2, transform2);
    ca.addComponent(e3, transform3);
    ca.addComponent(e4, transform4);
    ca.dump();

    Transform deleted = ca.removeComponent(e4);

    ca.dump();
    deleted.dump();

}