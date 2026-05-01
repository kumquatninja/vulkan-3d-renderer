#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace KQ {
    class WindowManager {
    public:
        WindowManager() = default;

        ~WindowManager() {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }

        void Init(int width, int height, const char* title, void* userPointer);

        void* GetWindowUserPointer()
        {
            glfwGetWindowUserPointer(m_Window);
        }

        void SetFramebufferSizeCallback(GLFWframebuffersizefun callback) {
            glfwSetFramebufferSizeCallback(WindowManager::m_Window, callback);
        }

        GLFWwindow* GetWindow() {
            return m_Window;
        }

        bool ShouldWindowClose() {
            return glfwWindowShouldClose(m_Window);
        }

        void PollEvents() {
            glfwPollEvents();
        }

        void GetFramebufferSize(int* width, int* height) {
            glfwGetFramebufferSize(m_Window, width, height);
        }

        void WaitEvents() {
            glfwWaitEvents();
        }

        void ResetResizedFlag() { m_Resized = false; }

        bool WasResized() const { return m_Resized; }

    private:
        GLFWwindow* m_Window;
        int m_Width, m_Height;
        bool m_Resized = false;

        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}