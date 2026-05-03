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

            static glm::vec2 GetMousePosition() { return m_MousePos; }
            static glm::vec2 GetMouseOffset() { return m_MouseOffset; }

            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

        private:
            static std::unordered_map<int, bool> m_Keys;
            static glm::vec2 m_MousePos;
            static glm::vec2 m_LastMousePos;
            static glm::vec2 m_MouseOffset;
            static bool m_FirstMouse;
    };
}
