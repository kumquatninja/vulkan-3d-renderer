#include "Camera.hpp"

namespace KQ
{
    void Camera::Update(float deltaTime) {
    }

    void Camera::UpdateCameraVectors() {
        m_Forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Forward.y = sin(glm::radians(m_Pitch));
        m_Forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

        m_Forward = glm::normalize(m_Forward);
        m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    }
} // namespace KQ
