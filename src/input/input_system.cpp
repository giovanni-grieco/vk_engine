// input.cpp
#include "input_system.hpp"
#include <iostream>

namespace engine
{

    std::unique_ptr<Input> Input::instance = nullptr;

    void Input::onKey(int key, bool down)
    {
        if (key >= 0 && key <= GLFW_KEY_LAST)
            keys_[key] = down;
        
        if (down)
            std::cout<<"pressed key: "<< key<<"\n";
        else
            std::cout<<"released key: "<< key<<"\n";
    }

    void Input::onMouseButton(int button, bool down)
    {
        if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST)
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

    bool Input::isKeyDown(int key) const { return keys_[key]; }
    bool Input::isMouseDown(int button) const { return mouseButtons_[button]; }
    bool Input::isKeyPressed(int key) const { return keys_[key] && !prevKeys_[key]; }
    bool Input::isKeyReleased(int key) const { return !keys_[key] && prevKeys_[key]; }
    glm::vec2 Input::mousePosition() const { return mousePos_; }
    glm::vec2 Input::mouseDelta() const { return mouseDelta_; }
    glm::vec2 Input::scrollDelta() const { return scroll_; }
}