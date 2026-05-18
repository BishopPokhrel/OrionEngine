#include "OEScene.h"
#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/ECS/OESystem.h>

namespace OrionEngine
{
	void OEScene::InitScene(const std::string& name) noexcept
	{
		m_OESceneName = name;
		m_Registry = CreateScope<ECS::OEECSRegistry>();
		m_Created = IsCreated();
	}

	void OEScene::DeleteScene() noexcept
	{
		m_Registry->DeleteRegistry();
	}

	void OEScene::UpdateSystems(ECS::OEECSRegistry& registry, float dt)
	{
		m_Registry->TransformSystem->Update(registry, dt);
		m_Registry->PhysicsSystem->Update(registry, dt);
		m_Registry->RenderSystem->Update(registry, dt);
	}
}