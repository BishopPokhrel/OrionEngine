#include "ORCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace OrionEngine
{
    namespace OrionRenderer
    {
        ORCamera::ORCamera(float left, float right, float top, float bottom) noexcept
        {
            const float originalHeight = top - bottom;
            const float originalWidth = right - left;

            if (originalHeight > 0.0f && originalWidth > 0.0f)
                m_AspectRatio = originalWidth / originalHeight;

            // Initialize the camera's direction toward its focal point.
            const glm::vec3 direction = glm::normalize(m_FocalPoint - m_Position);

            m_Yaw = std::atan2(direction.z, direction.x);
            m_Pitch = std::asin(direction.y);

            m_Distance = glm::length(m_FocalPoint - m_Position);

            RecalculateViewMatrix();
            RecalculateProjection();
        }

        void ORCamera::SetPosition(const glm::vec3& position) noexcept
        {
            m_Position = position;

            // Preserve the current orientation and distance.
            m_FocalPoint = m_Position + m_Forward * m_Distance;

            RecalculateViewMatrix();
        }

        void ORCamera::Translate(const glm::vec3& delta) noexcept
        {
            m_FocalPoint += delta;

            RecalculateViewMatrix();
        }

        void ORCamera::SetFocalPoint(const glm::vec3& focalPoint) noexcept
        {
            const glm::vec3 direction = focalPoint - m_Position;
            const float distance = glm::length(direction);

            if (distance <= 0.001f)
                return;

            m_FocalPoint = focalPoint;
            m_Distance = distance;

            const glm::vec3 normalizedDirection = direction / distance;

            m_Yaw = std::atan2(normalizedDirection.z, normalizedDirection.x);
            m_Pitch = std::asin(glm::clamp(normalizedDirection.y, -1.0f, 1.0f));

            RecalculateViewMatrix();
        }

        void ORCamera::Rotate(float deltaYaw, float deltaPitch) noexcept
        {
            m_Yaw += deltaYaw;
            m_Pitch += deltaPitch;

            constexpr float pitchLimit = glm::radians(89.0f);

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

        void ORCamera::SetDistance(float distance) noexcept
        {
            m_Distance = glm::clamp(distance, 0.5f, 500.0f);

            RecalculateViewMatrix();
        }

        void ORCamera::RecalculateProjection() noexcept
        {
            m_Projection = glm::perspective(
                glm::radians(m_FieldOfView),
                m_AspectRatio,
                0.1f,
                1000.0f
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

            m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

            // Maintain the camera's distance from its focal point.
            m_Position = m_FocalPoint - m_Forward * m_Distance;

            m_View = glm::lookAt(m_Position, m_FocalPoint, m_Up);

            m_ViewProjection = m_Projection * m_View;
        }
    }
}