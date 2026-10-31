
#define GLM_ENABLE_EXPERIMENTAL

#include "ORCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace OrionEngine
{
    namespace OrionRenderer
    {
        ORCamera::ORCamera(float left, float right, float top, float bottom) noexcept
        {
            m_View = glm::mat4(1.0f);
            m_ViewProjection = glm::mat4(1.0f);

            // Initial camera position
            m_Position = glm::vec3(0.0f, 5.0f, 10.0f);

            // Initial camera orientation
            m_Yaw = glm::radians(-90.0f);
            m_Pitch = glm::radians(-25.0f);

            m_Forward = glm::vec3(0.0f, 0.0f, -1.0f);
            m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
            m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

            // Original orthographic dimensions
            m_OrthographicHalfHeight = (top - bottom) * 0.5f;
            m_OrthographicCenterX = (left + right) * 0.5f;
            m_OrthographicCenterY = (top + bottom) * 0.5f;

            const float originalHeight = top - bottom;
            const float originalWidth = right - left;

            if (originalHeight > 0.0f && originalWidth > 0.0f)
                m_AspectRatio = originalWidth / originalHeight;

            RecalculateProjection();
            RecalculateViewMatrix();
        }

        void ORCamera::SetPosition(const glm::vec3& position) noexcept
        {
            m_Position = position;

            RecalculateViewMatrix();
        }

        void ORCamera::Translate(const glm::vec3& delta) noexcept
        {
            m_Position += delta;

            RecalculateViewMatrix();
        }

        void ORCamera::Rotate(float deltaYaw, float deltaPitch) noexcept
        {
            m_Yaw += deltaYaw;
            m_Pitch += deltaPitch;

            // Prevent camera flipping.
            const float pitchLimit = glm::radians(89.0f);

            m_Pitch = glm::clamp(
                m_Pitch,
                -pitchLimit,
                pitchLimit
            );

            RecalculateViewMatrix();
        }

        void ORCamera::SetAspectRatio(float aspectRatio) noexcept
        {
            if (aspectRatio <= 0.0f)
                return;

            m_AspectRatio = aspectRatio;

            RecalculateProjection();
        }

        void ORCamera::SetZoom(float zoom) noexcept
        {
            m_Zoom = glm::clamp(zoom, 0.1f, 10.0f);

            RecalculateProjection();
        }

        void ORCamera::RecalculateProjection() noexcept
        {
            const float halfHeight = m_OrthographicHalfHeight / m_Zoom;
            const float halfWidth = halfHeight * m_AspectRatio;

            m_Projection = glm::ortho(
                m_OrthographicCenterX - halfWidth,
                m_OrthographicCenterX + halfWidth,
                m_OrthographicCenterY - halfHeight,
                m_OrthographicCenterY + halfHeight,
                0.1f,
                500.0f
            );

            m_ViewProjection = m_Projection * m_View;
        }

        void ORCamera::RecalculateViewMatrix() noexcept
        {
            glm::vec3 forward;

            forward.x = std::cos(m_Yaw) * std::cos(m_Pitch);
            forward.y = std::sin(m_Pitch);
            forward.z = std::sin(m_Yaw) * std::cos(m_Pitch);

            m_Forward = glm::normalize(forward);

            const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

            m_Right = glm::normalize(
                glm::cross(m_Forward, worldUp)
            );

            m_Up = glm::normalize(
                glm::cross(m_Right, m_Forward)
            );

            m_View = glm::lookAt(
                m_Position,
                m_Position + m_Forward,
                m_Up
            );

            m_ViewProjection = m_Projection * m_View;
        }
    }
}
