#define GLM_ENABLE_EXPERIMENTAL

#include "ORCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		ORCamera::ORCamera(float left, float right, float top, float bottom) noexcept
		{
			glm::mat4 P = glm::ortho(left, right, bottom, top);
			m_Projection = P;

			m_View = glm::mat4(1.0f);
			m_ViewProjection = glm::mat4(1.0f);
			m_Position = glm::vec3(0.0f);
			m_Yaw = 0.0f;
			m_Pitch = 0.0f;
			m_Forward = glm::vec3(1.0f);
			m_Right = glm::vec3(1.0f);
			m_Up = glm::vec3(1.0f);

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

			// prevent fliping
			const float limit = glm::radians(89.0f);
			m_Pitch = glm::clamp(m_Pitch, -limit, limit);

			RecalculateViewMatrix();
		}

		void ORCamera::RecalculateViewMatrix() noexcept
		{
			glm::vec3 forward;
			forward.x = cos(m_Yaw) * cos(m_Pitch);
			forward.y = sin(m_Pitch);
			forward.z = sin(m_Yaw) * cos(m_Pitch);
			m_Forward = glm::normalize(forward);

			glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f };
			m_Right = glm::normalize(glm::cross(m_Forward, worldUp));
			m_Up = glm::cross(m_Right, m_Forward);

			m_View = glm::lookAt(
				m_Position,
				m_Position + m_Forward,
				m_Up
			);

			m_ViewProjection = m_Projection * m_View;
		}
	}
}