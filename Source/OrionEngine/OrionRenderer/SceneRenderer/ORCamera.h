
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

            // Camera rotation
            void Rotate(float deltaYaw, float deltaPitch) noexcept;

            // Projection settings
            void SetAspectRatio(float aspectRatio) noexcept;
            void SetZoom(float zoom) noexcept;

            // Camera matrices
            const glm::mat4& GetProjection() const noexcept { return m_Projection; }
            const glm::mat4& GetView() const noexcept { return m_View; }
            const glm::mat4& GetViewProjection() const noexcept { return m_ViewProjection; }

            // Camera properties
            const glm::vec3& GetPosition() const noexcept { return m_Position; }

            float GetYaw() const noexcept { return m_Yaw; }
            float GetPitch() const noexcept { return m_Pitch; }
            float GetZoom() const noexcept { return m_Zoom; }

            // Camera directions
            const glm::vec3& GetForward() const noexcept { return m_Forward; }
            const glm::vec3& GetRight() const noexcept { return m_Right; }
            const glm::vec3& GetUp() const noexcept { return m_Up; }

        private:
            void RecalculateViewMatrix() noexcept;
            void RecalculateProjection() noexcept;

        private:
            glm::mat4 m_Projection;
            glm::mat4 m_View;
            glm::mat4 m_ViewProjection;

            glm::vec3 m_Position;

            float m_Yaw;
            float m_Pitch;

            glm::vec3 m_Forward;
            glm::vec3 m_Right;
            glm::vec3 m_Up;

            // Orthographic projection settings
            float m_OrthographicHalfHeight = 1.0f;
            float m_OrthographicCenterX = 0.0f;
            float m_OrthographicCenterY = 0.0f;

            float m_AspectRatio = 16.0f / 9.0f;
            float m_Zoom = 1.0f;
        };
    }
}
