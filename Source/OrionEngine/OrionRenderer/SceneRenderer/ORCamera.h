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
			
			void SetPosition(const glm::vec3& position) noexcept;
			void Translate(const glm::vec3& delta) noexcept;
			void Rotate(float deltaYaw, float deltaPitch) noexcept;

			const glm::mat4& GetProjection() const noexcept { return m_Projection; }
			const glm::mat4& GetView() const noexcept { return m_View; }
			const glm::mat4& GetViewProjection() const noexcept { return m_ViewProjection; }
			const glm::vec3& GetPosition() const noexcept { return m_Position; }

			const float GetYaw() const noexcept { return m_Yaw; }
			const float GetPitch() const noexcept { return m_Pitch; }

			const glm::vec3& GetForward() const noexcept { return m_Forward; }
			const glm::vec3& GetRight() const noexcept { return m_Right; }
			const glm::vec3& GetUp() const noexcept { return m_Up; }
			
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

			void RecalculateViewMatrix() noexcept;
		};
	}
}