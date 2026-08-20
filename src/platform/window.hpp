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
            bool frameBufferResizeFlag = false;
            Window(int width, int height, const std::string& title);
            ~Window();

            std::vector<const char *> getRequiredExtensions();
            BufferSize getBufferSize();
            static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

            bool shouldWindowClose();
            void setWindowTitle(std::string title);
            void pollEvents();
    };
}