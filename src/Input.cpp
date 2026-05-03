#include "Input.hpp"

namespace KQ {
    std::unordered_map<int, bool> Input::m_Keys;
    glm::vec2 Input::m_MousePos = {0.0f, 0.0f};
    glm::vec2 Input::m_LastMousePos = {400.0f, 300.0f};
    glm::vec2 Input::m_MouseOffset = {0.0f, 0.0f};
    bool Input::m_FirstMouse = true;

    void Input::Update() {}

    void Input::IsKeyReleased(int keycode) {}

    bool Input::IsKeyPressed(int keycode) {
        return m_Keys[keycode];
    }

    void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            m_Keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            m_Keys[key] = false;
        }
    }

    void Input::MouseCallback(GLFWwindow* window, double xPos, double yPos) {
        m_MousePos = { (float)xPos, (float)yPos };

        if (m_FirstMouse) {
            m_LastMousePos = m_MousePos;
            m_FirstMouse = false;
        }

        m_MouseOffset.x = m_MouseOffset.x - m_LastMousePos.x;
        m_MouseOffset.y = m_LastMousePos.y - m_MousePos.y;
        m_LastMousePos = m_MousePos;
    }
}