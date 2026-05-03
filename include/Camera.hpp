
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULTS_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace KQ {
    class Camera {
        public:
            Camera(glm::vec3 target = glm::vec3(0.0f), float distance = 5.0f)
                : m_TargetPos(target), m_Distance(distance)
            {
                m_Yaw = -90.0f;
                m_Pitch = 40.0f;
                m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

                UpdateCameraVectors();
            }

            ~Camera() = default;

            void Update(float deltaTime);

            const glm::vec3& GetPosition() const { return m_Pos; }
            const glm::vec3& GetForward() const { return m_Forward; }
            const glm::vec3& GetRight() const { return m_Right; }
            const glm::vec3& GetUp() const { return m_Up; }

            void Move(const glm::vec3& displacement) {
                m_Pos += displacement;
            }

        private:
            float m_Yaw, m_Pitch;
            float m_Distance;
            glm::vec3 m_WorldUp;
            glm::vec3 m_TargetPos;

            glm::vec3 m_Pos;
            glm::vec3 m_Forward;
            glm::vec3 m_Right;
            glm::vec3 m_Up;

            void UpdateCameraVectors();
    };
}