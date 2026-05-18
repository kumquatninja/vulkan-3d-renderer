#pragma once

#include <chrono>

namespace KQ {
    struct Time
    {
        public:
            float deltaTime = 0.0f;
            float timeScale = 1.0f;

            void Update()
            {
                auto currentFrameTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float, std::chrono::seconds::period> elapsed = currentFrameTime - m_LastFrameTime;
                m_LastFrameTime = currentFrameTime;

                deltaTime = elapsed.count() * timeScale;
            }
        private:
            std::chrono::high_resolution_clock::time_point m_LastFrameTime = std::chrono::high_resolution_clock::now();
    };
}