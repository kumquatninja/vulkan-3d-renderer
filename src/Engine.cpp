#include "Engine.hpp"
#include "Input.hpp"
#include "SceneLoader.hpp"

#include <iostream>

namespace KQ {
    void Engine::Run() {
        LoadConfig();
        InitWindow();
        InitScene();
        InitRenderer();
        MainLoop();
        Cleanup();
    }

    void Engine::LoadConfig() {
        m_Config = LoadEngineConfig("config.ini");
    }

    void Engine::InitWindow() {
        m_WindowManager.Init(m_Config.windowWidth, m_Config.windowHeight, "KQuat Engine", this);
    }

    void Engine::InitScene() {
        m_Scene.gameObjects.clear();

        KQ::SceneDefinition sceneDef;
        if (!KQ::SceneLoader::LoadScene("scenes/test_scene.json", sceneDef)) {
            std::cerr << "Scene could not be loaded" << std::endl;

            return;
        }

        for (auto& object : sceneDef.objects) {
            auto& gameObject = m_Scene.AddGameObject();
            gameObject.modelPath = object.modelPath;
            gameObject.texturePath = object.texturePath;
            gameObject.position = object.position;
            gameObject.rotation = glm::radians(object.rotation);
            gameObject.scale = object.scale;
        }
    }

    void Engine::InitRenderer() {
        m_Renderer.Init(m_WindowManager.GetWindow());
    }

    void Engine::MainLoop() {
        m_Renderer.LoadScene(m_Scene);

        while (!m_WindowManager.ShouldWindowClose()) {
            m_WindowManager.PollEvents();
            KQ::Input::Update();
            m_Time.Update();
            Update(m_Time.deltaTime);
            Render();
        }

        vkDeviceWaitIdle(*m_Renderer.GetDevice());
    }

    void Engine::Update(float deltaTime) {
        ProcessInput(deltaTime);

        if (!m_Scene.gameObjects.empty()) {
            constexpr float ROTATION_SPEED = 0.02f;
            for (auto& gameObject : m_Scene.gameObjects)
            {
                gameObject.rotation.z += glm::degrees(ROTATION_SPEED * deltaTime);
            }
        }
    }

    void Engine::Render() {
        m_Renderer.DrawFrame(m_Camera, m_Scene);
    }

    void Engine::Cleanup() {
        m_Renderer.Cleanup(m_Scene);
        m_WindowManager.Cleanup();
        m_Scene.Cleanup();
    }

    void Engine::ProcessInput(float deltaTime) {
        float moveSpeed = 2.5f * deltaTime;
        const float shiftModifier = 2.0f;
        const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        if (KQ::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            auto mouseOffset = KQ::Input::GetMouseOffset();
            auto rotation = mouseOffset * m_Config.lookSensitivity;
            m_Camera.Rotate(rotation);
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            moveSpeed *= shiftModifier;
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_W)) {
            m_Camera.Move(m_Camera.GetForward() * moveSpeed);
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_S)) {
            m_Camera.Move(m_Camera.GetForward() * -moveSpeed);
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_A)) {
            m_Camera.Move(m_Camera.GetRight() * -moveSpeed);
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_D)) {
            m_Camera.Move(m_Camera.GetRight() * moveSpeed);
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_SPACE)) {
            m_Camera.Move(worldUp * moveSpeed);
        }

        if (KQ::Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
            m_Camera.Move(worldUp * -moveSpeed);
        }
    }
}
