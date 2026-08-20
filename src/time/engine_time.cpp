#include "engine_time.hpp"

namespace engine
{
    std::unique_ptr<EngineTime> EngineTime::instance = nullptr;

    EngineTime &EngineTime::getInstance()
    {
        if (instance == nullptr)
        {
            instance = std::make_unique<EngineTime>();
        }
        return *instance;
    }

    void EngineTime::beginFrame()
    {
        auto now = std::chrono::steady_clock::now();

        // Avoid a huge dt spike on the very first frame.
        if (firstFrame_)
        {
            lastTime_ = now;
            firstFrame_ = false;
            deltaTime_ = 0.0f;
            return;
        }

        float dt = std::chrono::duration<float>(now - lastTime_).count();
        lastTime_ = now;

        deltaTime_ = dt;
        totalTime_ += dt;

        if (dt > 0.0f)
        {
            float fps = 1.0f / dt;
            fps_ = (fps_ == 0.0f) ? fps : fps_ * 0.95f + fps * 0.05f;
        }
    }

    float EngineTime::deltaTime() const { return deltaTime_; }
    float EngineTime::totalTime() const { return totalTime_; }
    float EngineTime::fps() const { return fps_; }
}