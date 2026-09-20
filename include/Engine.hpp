#pragma once

#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Time.hpp"
#include "Config.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include <mutex>
#include "noclip.h"

namespace KQ {
    class Engine {
    public:
        void Run();

    private:
        void LoadConfig();
        void InitWindow();
        void InitScene();
        void InitConsole();
        void PollConsoleInput();
        void CheckConsoleCommandQueue();
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
        noclip::console m_Console;

        std::mutex m_ConsoleQueueMutex;
        std::string m_ConsoleQueue;
        std::thread m_InputCheckingThread;
    };
}
