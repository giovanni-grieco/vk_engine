#include "window.hpp"
#include "input/input_system.hpp"

namespace engine
{

    Window::Window(int width, int height, const std::string &title)
    {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

        glfwSetKeyCallback(window, [](GLFWwindow *, int, int scancode, int action, int)
                           { Input::getInstance().onKey(scancode, action == GLFW_PRESS || action == GLFW_REPEAT); });
        glfwSetMouseButtonCallback(window, [](GLFWwindow *, int button, int action, int)
                                   { Input::getInstance().onMouseButton(button, action == GLFW_PRESS); });
        glfwSetCursorPosCallback(window, [](GLFWwindow *, double x, double y)
                                 { Input::getInstance().onCursorPos(x, y); });
        glfwSetScrollCallback(window, [](GLFWwindow *, double x, double y)
                              { Input::getInstance().onScroll(x, y); });
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    std::vector<const char *> Window::getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }

    BufferSize Window::getBufferSize()
    {
        BufferSize result;
        int tempWidth, tempHeight;
        glfwGetFramebufferSize(this->window, &tempWidth, &tempHeight);
        result.width = tempWidth;
        result.height = tempHeight;
        return result;
    }

    void Window::frameBufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto windowObj = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowObj->frameBufferResizeFlag = true;
    }

    bool Window::shouldWindowClose()
    {
        return glfwWindowShouldClose(window) != 0;
    }

    void Window::setWindowTitle(std::string title)
    {
        glfwSetWindowTitle(window, title.c_str());
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }
}