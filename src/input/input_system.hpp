// input.hpp
#pragma once
#include <array>
#include <glm/vec2.hpp>
#include <memory>
#include <unordered_map>

namespace engine
{
    // Engine-level, platform-independent key codes. The input system maps these
    // to the underlying platform scancode internally, keeping GLFW hidden.
    enum class Key : int
    {
        Unknown = -1,
        W, S, A, D, Q, E,
        Up, Down, Left, Right,
        Count, Space
    };

    enum class MouseButton : int
    {
        Left, Right, Middle,
        Button4, Button5, Button6, Button7, Button8,
        Count
    };

    class Input
    {
    public:

        static Input &getInstance()
        {
            if (instance == nullptr)
            {
                instance = std::unique_ptr<Input>(std::make_unique<Input>());
            }
            return *instance;
        }

        Input() = default;

        // Internal: called by the window's GLFW callbacks (same thread as glfwPollEvents).
        void onKey(int scancode, bool down);
        void onMouseButton(int button, bool down);
        void onCursorPos(double x, double y);
        void onScroll(double xOffset, double yOffset);

        // Call at the END of each frame (after you've read input).
        void endFrame();

        // Continuous state (held)
        bool isKeyDown(Key key) const;
        bool isMouseDown(MouseButton button) const;

        // Edge detection (this-frame transitions)
        bool isKeyPressed(Key key) const;  // rising edge
        bool isKeyReleased(Key key) const; // falling edge

        glm::vec2 mousePosition() const;
        glm::vec2 mouseDelta() const; // per-frame movement
        glm::vec2 scrollDelta() const;

    private:
        static std::unique_ptr<Input> instance;

        // Key state is indexed by platform scancode (physical key position).
        // Scancodes are platform-specific and sparse, so a fixed array would be unsafe.
        std::unordered_map<int, bool> keys_{};
        std::unordered_map<int, bool> prevKeys_{};
        std::array<bool, static_cast<std::size_t>(MouseButton::Count)> mouseButtons_{};
        std::array<bool, static_cast<std::size_t>(MouseButton::Count)> prevMouseButtons_{};

        glm::vec2 mousePos_{};
        glm::vec2 prevMousePos_{};
        glm::vec2 mouseDelta_{};
        glm::vec2 scroll_{};
        bool firstMouse_ = true;
    };
}