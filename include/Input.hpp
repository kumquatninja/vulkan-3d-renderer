#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace KQ {
    class Input {
        public:
            static void Update();

            static bool IsKeyPressed(int keycode);
            static void IsKeyReleased(int keycode);
            static bool IsMouseButtonPressed(int button);

            static glm::vec2 GetMousePosition() { return m_MousePos; }
            static glm::vec2 GetMouseOffset() {
                glm::vec2 offset = m_MouseOffset;
                m_MouseOffset = glm::vec2(0.0f, 0.0f);
                return offset;
            }

            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
            static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        private:
            static std::unordered_map<int, bool> m_Keys;
            static std::unordered_map<int, bool> m_Buttons;
            static glm::vec2 m_MousePos;
            static glm::vec2 m_LastMousePos;
            static glm::vec2 m_MouseOffset;
            static bool m_FirstMouse;
    };
}
