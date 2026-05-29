#include "OEScene.h"
#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/ECS/OESystem.h>

namespace OrionEngine
{
	void OEScene::InitScene(const std::string& name) noexcept
	{
		m_OESceneName = name;
		m_Registry = CreateScope<ECS::OEECSRegistry>();
		m_Created = true;
	}

	void OEScene::DeleteScene() noexcept
	{
		m_Registry->DeleteRegistry();
	}
}