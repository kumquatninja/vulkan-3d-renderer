#include "Camera.hpp"

namespace KQ
{
    void Camera::Update(float deltaTime) {
    }

    void Camera::UpdateCameraVectors() {
        glm::vec3 offset;
        offset.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)) * m_Distance;
        offset.y = sin(glm::radians(m_Pitch)) * m_Distance;
        offset.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)) * m_Distance;

        m_Pos = m_TargetPos + offset;
        m_Forward = glm::normalize(m_TargetPos - m_Pos);
        m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    }
} // namespace KQ
