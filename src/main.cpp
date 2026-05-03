#include "Vertex.h"
#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "Camera.hpp"

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		InitWindow();
		InitVulkan();
		MainLoop(); // blocking call
		Cleanup();
	}

private:
	KQ::WindowManager m_WindowManager;
	KQ::Renderer m_Renderer;
	KQ::Camera m_Camera;

	void InitWindow() {
		m_WindowManager.Init(WIDTH, HEIGHT, "KQuat Engine", this);
	}

	void InitVulkan() {
		m_Renderer.Init(m_WindowManager.GetWindow());
	}

	void MainLoop() {
		double lastFrameTime = glfwGetTime();
		double currentFrameTime = glfwGetTime();
		double deltaTime = 0.0f;

		while (!m_WindowManager.ShouldWindowClose()) {
			m_WindowManager.PollEvents();

			currentFrameTime = glfwGetTime();
			deltaTime = currentFrameTime - lastFrameTime;
			ProcessInput((float)deltaTime);
			m_Renderer.DrawFrame(m_Camera.GetPosition(), m_Camera.GetForward(), m_Camera.GetUp());
			lastFrameTime = currentFrameTime;
		}

		vkDeviceWaitIdle(*m_Renderer.GetDevice());
	}

	void Cleanup() {
		m_Renderer.Cleanup();
		m_WindowManager.Cleanup();
	}

	void ProcessInput(float deltaTime) {
		float moveSpeed = 2.5f * deltaTime;
		const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
	HelloTriangleApplication app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}