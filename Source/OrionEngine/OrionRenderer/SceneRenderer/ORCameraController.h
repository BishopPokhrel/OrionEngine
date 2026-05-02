#pragma once

#include <OrionEngine/Core/OEInputSystem.h>
#include <OrionEngine/Core/OERef.h>

#include "ORCamera.h"

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORCameraController
		{
		public:

			ORCameraController(ORCamera* camera, OEInputSystem* input, float speed) noexcept;

			void OnUpdate(float dt) noexcept;

		private:

			ORCamera* m_Camera;
			float m_Speed;
			float m_Sensitivity;

			OEInputSystem* m_InputSystem;
		};
	}
}