#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

namespace engine
{

    struct BufferSize{
        int width;
        int height;
    };
    
    class Window{
        public:
            GLFWwindow *window;
            Window(int width, int height, const std::string& title);
            ~Window();

            std::vector<const char *> getRequiredExtensions();
            BufferSize getBufferSize();

    };
}