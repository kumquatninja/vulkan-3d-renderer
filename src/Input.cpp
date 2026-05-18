#include "Input.hpp"

namespace KQ {
    std::unordered_map<int, bool> Input::m_Keys;
    std::unordered_map<int, bool> Input::m_Buttons;
    glm::vec2 Input::m_MousePos;
    glm::vec2 Input::m_LastMousePos;
    glm::vec2 Input::m_MouseOffset;
    bool Input::m_FirstMouse = true;

    void Input::Update() {
        if (m_FirstMouse) {
            m_LastMousePos = m_MousePos;
            m_FirstMouse = false;
        }

        m_MouseOffset.x = m_MousePos.x - m_LastMousePos.x;
        m_MouseOffset.y = m_LastMousePos.y - m_MousePos.y; // +Y is down in GLFW
        m_LastMousePos = m_MousePos;
    }

    bool Input::IsKeyReleased(int keycode) { return false; }

    void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            m_Keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            m_Keys[key] = false;
        }
    }

    void Input::MouseCallback(GLFWwindow* window, double xPos, double yPos) {
        m_MousePos = { (float)xPos, (float)yPos };
    }

    void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            m_Buttons[button] = true;
        } else if (action == GLFW_RELEASE) {
            m_Buttons[button] = false;
        }
    }
}