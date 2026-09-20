#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "noclip.h"

namespace KQ {
    class WindowManager {
    public:
        WindowManager() = default;

        ~WindowManager() {}

        void Init(const int& width, const int& height, const char* title, void* userPointer);
        void BindConsoleCommands(noclip::console& console);

        inline void Cleanup() {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }

        inline void GetWindowUserPointer()
        {
            glfwGetWindowUserPointer(m_Window);
        }

        inline void SetFramebufferSizeCallback(GLFWframebuffersizefun callback) {
            glfwSetFramebufferSizeCallback(WindowManager::m_Window, callback);
        }

        inline GLFWwindow* GetWindow() {
            return m_Window;
        }

        inline bool ShouldWindowClose() {
            return glfwWindowShouldClose(m_Window);
        }

        inline void PollEvents() {
            glfwPollEvents();
        }

        inline void GetFramebufferSize(int* width, int* height) {
            glfwGetFramebufferSize(m_Window, width, height);
        }

        inline void WaitEvents() {
            glfwWaitEvents();
        }

        inline void ResetResizedFlag() { m_Resized = false; }

        inline bool WasResized() const { return m_Resized; }

    private:
        GLFWwindow* m_Window;
        int m_Width, m_Height;
        bool m_Resized = false;

        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
        inline void SetWindowShouldClose() { glfwSetWindowShouldClose(m_Window, GLFW_TRUE); }
    };
}