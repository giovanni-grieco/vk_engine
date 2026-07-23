
#include <iostream>

namespace engine{
    struct Transform{
        float x,y,z;
        float pitch,yaw,roll;

        void dump(){
            std::cout << "Transform: \n";
            std::cout << "Position: (" << x << ", " << y << ", " << z << ")\n";
            std::cout << "Rotation: (" << pitch << ", " << yaw << ", " << roll << ")\n";
        }
    };
}