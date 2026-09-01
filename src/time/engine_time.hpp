#pragma once
#include <chrono>
#include <memory>

namespace engine
{
    // Centralized game-time source.
    // Call beginFrame() once at the top of the main loop, then read
    // deltaTime() from anywhere (systems, renderer, etc.).
    class EngineTime
    {
    public:
        static EngineTime &getInstance();

        EngineTime() = default;
        // Computes this frame's dt from the steady clock and advances time.
        // Call exactly once per frame, before systems read deltaTime().
        void beginFrame();

        // dt of the most recently completed frame, in seconds.
        float deltaTime() const;

        // Total elapsed game time in seconds.
        float totalTime() const;

        // Smoothed FPS (exponential moving average).
        float fps() const;

    private:
        static std::unique_ptr<EngineTime> instance;

        std::chrono::steady_clock::time_point lastTime_{};
        float deltaTime_ = 0.0f;
        float totalTime_ = 0.0f;
        float fps_ = 0.0f;
        bool firstFrame_ = true;
    };
}