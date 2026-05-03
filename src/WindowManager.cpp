#include "WindowManager.hpp"
#include "Input.hpp"

namespace KQ {
    void WindowManager::Init(int width, int height, const char* title, void* userPointer) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Width = width;
        m_Height = height;

        WindowManager::m_Window = glfwCreateWindow(m_Width, m_Height, title, nullptr, nullptr);
        glfwSetWindowUserPointer(WindowManager::m_Window, userPointer);
        glfwSetFramebufferSizeCallback(WindowManager::m_Window, FramebufferResizeCallback);

        glfwSetKeyCallback(m_Window, Input::KeyCallback);
        glfwSetCursorPosCallback(m_Window, Input::MouseCallback);

        // glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void WindowManager::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto manager = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));

        if (manager) {
            manager->m_Resized = true;
        }
    }
}