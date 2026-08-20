#include "window.hpp"

namespace engine{

    Window::Window(int width, int height, const std::string& title) {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    

    std::vector<const char *> Window::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }

    BufferSize Window::getBufferSize(){
        BufferSize result;
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(this->window, &tempWidth, &tempHeight);
        result.width=tempWidth;
        result.height=tempHeight;
        return result;
    }

    void Window::frameBufferResizeCallback(GLFWwindow* window, int width, int height){
        auto windowObj = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        windowObj->frameBufferResizeFlag = true;
    }

    bool Window::shouldWindowClose(){
        return glfwWindowShouldClose(window) != 0;
    }

    void Window::setWindowTitle(std::string title){
        glfwSetWindowTitle(window, title.c_str());
    }
}