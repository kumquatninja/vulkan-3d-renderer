#include "Vertex.h"
#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Time.hpp"
#include "Config.hpp"

#include <iostream>

// const uint32_t WIDTH = 800;
// const uint32_t HEIGHT = 600;

class KQuatApp {
public:
	void run() {
		LoadConfig();
		InitWindow();
		InitScene();
		InitVulkan();
		MainLoop(); // blocking call
		Cleanup();
	}

private:
	KQ::WindowManager m_WindowManager;
	KQ::Renderer m_Renderer;
	KQ::Camera m_Camera;
	KQ::Scene m_Scene;
	KQ::Time m_Time;
	KQ::EngineConfig m_Config;

	void LoadConfig() {
		m_Config = KQ::LoadEngineConfig("config.ini");
	}

	void InitWindow() {
		m_WindowManager.Init(m_Config.windowWidth, m_Config.windowHeight, "KQuat Engine", this);
	}

	void InitScene() {
		m_Scene.gameObjects.emplace_back();
		m_Scene.gameObjects.emplace_back();
		m_Scene.gameObjects.emplace_back();

		// Object 1 - Center
		m_Scene.gameObjects[0].position = {0.0f, 0.0f, 0.0f};
		m_Scene.gameObjects[0].rotation = {glm::radians(-90.0f), 0.0f, 0.0f};
		m_Scene.gameObjects[0].scale = {1.0f, 1.0f, 1.0f};

		// Object 2 - Left
		m_Scene.gameObjects[1].position = {-2.0f, 0.0f, -1.0f};
		m_Scene.gameObjects[1].rotation = {glm::radians(-90.0f), glm::radians(45.0f), 0.0f};
		m_Scene.gameObjects[1].scale = {0.75f, 0.75f, 0.75f};

		// Object 3 - Right
		m_Scene.gameObjects[2].position = {2.0f, 0.0f, -1.0f};
		m_Scene.gameObjects[2].rotation = {glm::radians(-90.0f), glm::radians(-45.0f), 0.0f};
		m_Scene.gameObjects[2].scale = {0.75f, 0.75f, 0.75f};
	}

	void InitVulkan() {
		m_Renderer.Init(m_WindowManager.GetWindow());
	}

	void MainLoop() {
		double lastFrameTime = glfwGetTime();
		double currentFrameTime = glfwGetTime();
		double deltaTime = 0.0f;
		const float SPEED = 0.02f;

		while (!m_WindowManager.ShouldWindowClose()) {
			m_WindowManager.PollEvents();

			currentFrameTime = glfwGetTime();
			deltaTime = currentFrameTime - lastFrameTime;
			KQ::Input::Update();
			ProcessInput((float)deltaTime);
			m_Time.Update();
			m_Scene.gameObjects[0].rotation.z += glm::degrees(SPEED * m_Time.deltaTime);
			m_Renderer.DrawFrame(m_Camera, m_Scene);
			lastFrameTime = currentFrameTime;
		}

		vkDeviceWaitIdle(*m_Renderer.GetDevice());
	}

	void Cleanup() {
		m_Renderer.Cleanup();
		m_WindowManager.Cleanup();
		m_Scene.Cleanup();
	}

	void ProcessInput(float deltaTime) {
		float moveSpeed = 2.5f * deltaTime;
		float moveSpeedShiftModifier = 2.0f;
		const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		if (KQ::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			auto mouseOffset = KQ::Input::GetMouseOffset();
			auto rotation = mouseOffset * m_Config.lookSensitivity;
			m_Camera.Rotate(rotation);
		}

		if (KQ::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
			moveSpeed *= moveSpeedShiftModifier;
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
};

int main() {
	KQuatApp app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}