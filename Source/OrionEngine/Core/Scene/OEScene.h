#pragma once

#include <string>
#include <vector>

#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

namespace OrionEngine
{
	class OEScene
	{
	public:

		explicit OEScene(const std::string& name) noexcept;

	private:
		std::string m_OESceneName;
		ECS::OEECSRegistry m_Registry;
	};
}