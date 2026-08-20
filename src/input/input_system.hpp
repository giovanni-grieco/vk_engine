// input.hpp
#pragma once
#include <array>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>
#include <memory>

namespace engine
{
    class Input
    {
    public:
        static Input& getInstance()
        {
            if (instance == nullptr)
            {
                instance = std::unique_ptr<Input>(std::make_unique<Input>());
            }
            return *instance;
        }

        // Called by GLFW callbacks (thread-safe enough: same thread as glfwPollEvents)
        void onKey(int key, bool down);
        void onMouseButton(int button, bool down);
        void onCursorPos(double x, double y);
        void onScroll(double xOffset, double yOffset);

        // Call at the END of each frame (after you've read input).
        void endFrame();

        // Continuous state (held)
        bool isKeyDown(int key) const;
        bool isMouseDown(int button) const;

        // Edge detection (this-frame transitions)
        bool isKeyPressed(int key) const;   // rising edge
        bool isKeyReleased(int key) const;  // falling edge

        glm::vec2 mousePosition() const;
        glm::vec2 mouseDelta() const;       // per-frame movement
        glm::vec2 scrollDelta() const;

    private:

        static std::unique_ptr<Input> instance;

        Input() = default;

        std::array<bool, GLFW_KEY_LAST + 1> keys_{};
        std::array<bool, GLFW_KEY_LAST + 1> prevKeys_{};
        std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseButtons_{};
        std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> prevMouseButtons_{};

        glm::vec2 mousePos_{};
        glm::vec2 prevMousePos_{};
        glm::vec2 mouseDelta_{};
        glm::vec2 scroll_{};
        bool firstMouse_ = true;
    };
}