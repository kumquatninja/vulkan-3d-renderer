#include "Camera.hpp"

namespace KQ
{
    void Camera::Update(float deltaTime) {
    }

    void Camera::UpdateCameraVectors() {
        float radYaw = glm::radians(m_Yaw);
        float radPitch = glm::radians(m_Pitch);

        m_Forward.x = cos(radYaw) * cos(radPitch);
        m_Forward.y = sin(radPitch);
        m_Forward.z = sin(radYaw) * cos(radPitch);

        m_Forward = glm::normalize(m_Forward);
        m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    }
} // namespace KQ
