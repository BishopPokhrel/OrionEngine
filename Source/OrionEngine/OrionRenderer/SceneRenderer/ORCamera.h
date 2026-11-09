#pragma once

#include <glm/glm.hpp>

namespace OrionEngine
{
    namespace OrionRenderer
    {
        class ORCamera
        {
        public:
            ORCamera(float left, float right, float top, float bottom) noexcept;

            // Camera position and movement
            void SetPosition(const glm::vec3& position) noexcept;
            void Translate(const glm::vec3& delta) noexcept;

            // Camera focal point
            void SetFocalPoint(const glm::vec3& focalPoint) noexcept;
            const glm::vec3& GetFocalPoint() const noexcept { return m_FocalPoint; }

            // Camera rotation
            void Rotate(float deltaYaw, float deltaPitch) noexcept;

            // Projection settings
            void SetAspectRatio(float aspectRatio) noexcept;
            void SetDistance(float distance) noexcept;

            // Camera matrices
            const glm::mat4& GetProjection() const noexcept { return m_Projection; }
            const glm::mat4& GetView() const noexcept { return m_View; }
            const glm::mat4& GetViewProjection() const noexcept { return m_ViewProjection; }

            // Camera properties
            const glm::vec3& GetPosition() const noexcept { return m_Position; }

            float GetYaw() const noexcept { return m_Yaw; }
            float GetPitch() const noexcept { return m_Pitch; }
            float GetDistance() const noexcept { return m_Distance; }

            // Camera directions
            const glm::vec3& GetForward() const noexcept { return m_Forward; }
            const glm::vec3& GetRight() const noexcept { return m_Right; }
            const glm::vec3& GetUp() const noexcept { return m_Up; }

        private:

            void RecalculateViewMatrix() noexcept;
            void RecalculateProjection() noexcept;

        private:
            glm::mat4 m_Projection{ 1.0f };
            glm::mat4 m_View{ 1.0f };
            glm::mat4 m_ViewProjection{ 1.0f };

            glm::vec3 m_Position{ 0.0f, 5.0f, 10.0f };
            glm::vec3 m_FocalPoint{ 0.0f };

            float m_Yaw = 0.0f;
            float m_Pitch = 0.0f;

            float m_Distance = 10.0f;

            glm::vec3 m_Forward{ 0.0f, 0.0f, -1.0f };
            glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
            glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };

            float m_AspectRatio = 16.0f / 9.0f;
            float m_FieldOfView = 45.0f;
        };
    }
}