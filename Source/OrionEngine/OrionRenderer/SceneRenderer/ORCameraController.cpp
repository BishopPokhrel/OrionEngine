#include "ORCameraController.h"

#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		ORCameraController::ORCameraController(ORCamera* camera, OEInputSystem* input, float speed) noexcept
		{
			m_Camera = camera;
			m_Speed = speed;

			m_InputSystem = input;

			m_InputSystem->RegisterKey("W");
			m_InputSystem->RegisterKey("A");
			m_InputSystem->RegisterKey("S");
			m_InputSystem->RegisterKey("D");
		}

		void ORCameraController::OnUpdate(float dt) noexcept
		{
			OE_CORE_ASSERT(m_Camera, "Camera inside Controller is null");
			glm::vec3 movement = glm::vec3(0.0f);

			if (m_InputSystem->IsKeyHeld("W"))
				movement -= m_Camera->GetForward();
			
			if (m_InputSystem->IsKeyHeld("S"))
				movement += m_Camera->GetForward();

			if (m_InputSystem->IsKeyHeld("A"))
				movement -= m_Camera->GetRight();

			if (m_InputSystem->IsKeyHeld("D"))
				movement += m_Camera->GetRight();

			movement *= m_Speed * dt;
			m_Camera->Translate(movement);
		}
	}
}