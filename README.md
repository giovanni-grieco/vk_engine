# Starfighter 
A small space ship demo made with a Custom Engine, using the Vulkan API

# Engine Architecture
The architecture of choice was a Entity Component System. This allows us to interact with any part of the state of the game from any angle/point by doing the opposite of encapsulation.

Entities are just IDs, Components are Data, and Systems are logic that act on that data.
The relative Managers store the relevant data. 

The Scene Manager stores entities that exists within a scene and their parent/child relationship. For example Entity b has a parent Entity a;

The Component Manager stores the components information relative to any Entity in existance at runtime. For example, it is able to fetch the "LocalTransformComponent" of a certain Entity e.

The System Manager runs all systems.

All of these managers are singleton accessible from anywhere within the codebase, and therefor every single entity and component can be manipulated from anywhere at runtime.

Another architectural pattern used is Layers. The engine is structured in layers where we have the ECS on top, we have a Vulkan "backend" at the bottom and in the middle we have a Renderer that translates things from ECS layer to the Vulkan layer.

This communication happens in the form of Draw Packets. Before any render pass, a batch of Draw Packets are issued containing all the revelant information for the Vulkan backend to use. So for one frame, there can be multiple Draw Packets. Each Draw Packets contains the data regarding the meshes, textures and matrix transforms computed by the CPU to render things correctly. Additional per-frame information is passed via Frame Info. It currently contains the view and projection matrices to be used in the shader.

Resources like Meshes and Textures are managed by the Vulkan backend. Once a resource is uploaded on GPU memory, it returns a Handle to reference it at the layers above.

# Fedora setup
```
sudo dnf install vulkan-tools
sudo dnf install vulkan-loader-devel

sudo dnf install vulkan-validation-layers
sudo dnf install mesa-vulkan-drivers vulkan-validation-layers-devel

sudo dnf install libXxf86vm-devel
sudo dnf install libXi-devel

sudo dnf install glfw-devel
sudo dnf install glm-devel

sudo dnf install glslc
```

# MacOS setup
**Download the VulkanSDK which will install glslc, MoltenVK and various libraries.**

Install glm and glfw via Brew
```
brew install glm glfw
```

# To run
To run with a DEBUG build type:
```
./debug_unix_build.sh
```

To run with a RELEASE build type:
```
./release_unix_build.sh
```



# Resources
[Vulkan Tutorial](https://vulkan-tutorial.com/)

[Vulkan Game Engine Tutorial](https://youtube.com/playlist?list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&si=v_nPlJEXTzS4EWST)

[Essence of linear algebra](https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab)

[Game Loop Article 1](https://gameprogrammingpatterns.com/game-loop.html)

[Game Loop Article 2](https://gafferongames.com/post/fix_your_timestep/)

[A Simple Entity Component System (ECS) \[C++\]](https://austinmorlan.com/posts/entity_component_system/)
