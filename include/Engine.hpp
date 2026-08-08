#pragma once

#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Time.hpp"
#include "Config.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

namespace KQ {
    class Engine {
    public:
        void Run();

    private:
        void LoadConfig();
        void InitWindow();
        void InitScene();
        void InitRenderer();
        void MainLoop();
        void Update(float deltaTime);
        void Render();
        void Cleanup();

        void ProcessInput(float deltaTime);

        WindowManager m_WindowManager;
        Renderer m_Renderer;
        Camera m_Camera;
        Scene m_Scene;
        Time m_Time;
        EngineConfig m_Config;
    };
}
