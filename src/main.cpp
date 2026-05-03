#include "Vertex.h"
#include "WindowManager.hpp"
#include "Renderer.hpp"
#include "Input.hpp"

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		m_CameraFront = glm::normalize(m_CameraFront);
		InitWindow();
		InitVulkan();
		MainLoop(); // blocking call
		Cleanup();
	}

private:
	KQ::WindowManager m_WindowManager;
	KQ::Renderer m_Renderer;

	glm::vec3 m_CameraPos	 = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec3 m_CameraFront  = glm::vec3(-2.0f, -2.0f, -2.0f);
	glm::vec3 m_CameraUp 	 = glm::vec3(0.0f, 1.0f, 0.0f);

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
			m_Renderer.DrawFrame(m_CameraPos, m_CameraFront, m_CameraUp);
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
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 right = glm::normalize(glm::cross(m_CameraFront, worldUp));

		if (KQ::Input::IsKeyPressed(GLFW_KEY_W)) {
			m_CameraPos += m_CameraFront * moveSpeed;
		}

		if (KQ::Input::IsKeyPressed(GLFW_KEY_S)) {
			m_CameraPos -= m_CameraFront * moveSpeed;
		}

		if (KQ::Input::IsKeyPressed(GLFW_KEY_A)) {
			m_CameraPos -= right * moveSpeed;

		}

		if (KQ::Input::IsKeyPressed(GLFW_KEY_D)) {
			m_CameraPos += right * moveSpeed;
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