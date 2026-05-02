#include "Vertex.h"
#include "WindowManager.hpp"
#include "Renderer.hpp"

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

	void InitWindow() {
		m_WindowManager.Init(WIDTH, HEIGHT, "KQuat Engine", this);
	}

	void InitVulkan() {
		m_Renderer.Init(m_WindowManager.GetWindow());
	}

	void MainLoop() {
		while (!m_WindowManager.ShouldWindowClose()) {
			m_WindowManager.PollEvents();
			m_Renderer.DrawFrame();
		}

		vkDeviceWaitIdle(*m_Renderer.GetDevice());
	}

	void Cleanup() {
		m_Renderer.Cleanup();
		m_WindowManager.~WindowManager();
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