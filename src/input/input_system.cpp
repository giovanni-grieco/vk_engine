// input.cpp
#include "input_system.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace engine
{

    std::unique_ptr<Input> Input::instance = nullptr;

    namespace
    {
        // Maps an engine key to the platform-independent GLFW key token.
        int glfwKeyToken(Key key)
        {
            switch (key)
            {
                case Key::W:     return GLFW_KEY_W;
                case Key::S:     return GLFW_KEY_S;
                case Key::A:     return GLFW_KEY_A;
                case Key::D:     return GLFW_KEY_D;
                case Key::Q:     return GLFW_KEY_Q;
                case Key::E:     return GLFW_KEY_E;
                case Key::Up:    return GLFW_KEY_UP;
                case Key::Down:  return GLFW_KEY_DOWN;
                case Key::Left:  return GLFW_KEY_LEFT;
                case Key::Right: return GLFW_KEY_RIGHT;
                case Key::Space: return GLFW_KEY_SPACE;
                default:         return GLFW_KEY_UNKNOWN;
            }
        }
    }

    void Input::onKey(int scancode, bool down)
    {
        keys_[scancode] = down;
    }

    void Input::onMouseButton(int button, bool down)
    {
        if (button >= 0 && button < static_cast<int>(MouseButton::Count))
            mouseButtons_[button] = down;
    }

    void Input::onCursorPos(double x, double y)
    {
        mousePos_ = {static_cast<float>(x), static_cast<float>(y)};
        if (firstMouse_)
        {
            prevMousePos_ = mousePos_;
            firstMouse_ = false;
        }
    }

    void Input::onScroll(double, double yOffset)
    {
        scroll_.y += static_cast<float>(yOffset);
    }

    void Input::endFrame()
    {
        prevKeys_ = keys_;
        prevMouseButtons_ = mouseButtons_;
        mouseDelta_ = mousePos_ - prevMousePos_;
        prevMousePos_ = mousePos_;
        scroll_ = glm::vec2(0.0f);
    }

    bool Input::isKeyDown(Key key) const
    {
        const int scancode = glfwGetKeyScancode(glfwKeyToken(key));
        const auto it = keys_.find(scancode);
        return it != keys_.end() && it->second;
    }
    bool Input::isMouseDown(MouseButton button) const
    {
        return mouseButtons_[static_cast<std::size_t>(button)];
    }
    bool Input::isKeyPressed(Key key) const
    {
        const int scancode = glfwGetKeyScancode(glfwKeyToken(key));
        const auto it = keys_.find(scancode);
        const bool down = it != keys_.end() && it->second;
        const auto prevIt = prevKeys_.find(scancode);
        const bool wasDown = prevIt != prevKeys_.end() && prevIt->second;
        return down && !wasDown;
    }
    bool Input::isKeyReleased(Key key) const
    {
        const int scancode = glfwGetKeyScancode(glfwKeyToken(key));
        const auto it = keys_.find(scancode);
        const bool down = it != keys_.end() && it->second;
        const auto prevIt = prevKeys_.find(scancode);
        const bool wasDown = prevIt != prevKeys_.end() && prevIt->second;
        return !down && wasDown;
    }
    glm::vec2 Input::mousePosition() const { return mousePos_; }
    glm::vec2 Input::mouseDelta() const { return mouseDelta_; }
    glm::vec2 Input::scrollDelta() const { return scroll_; }
}