#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULTS_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace KQ {
    class Camera {
        public:
            Camera() {
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

            void Rotate(const glm::vec2& offset) {
                m_Yaw += offset.x;
                m_Pitch += offset.y;

                UpdateCameraVectors();
            }

        private:
            float m_Yaw = -90.0f;
            float m_Pitch = -40.0f;
            glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);;

            glm::vec3 m_Pos = glm::vec3(0.0f, 2.0f, 2.0f);
            glm::vec3 m_Forward;
            glm::vec3 m_Right;
            glm::vec3 m_Up;

            void UpdateCameraVectors();
    };
}