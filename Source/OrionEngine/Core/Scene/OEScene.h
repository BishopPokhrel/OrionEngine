#pragma once

#include <string>
#include <vector>

#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>

namespace OrionEngine::ECS
{
	class OEECSRegistry;
}

namespace OrionEngine
{
	class OEScene
	{
	public:
		
		OEScene() = default;
		~OEScene() = default;

		void InitScene(const std::string& name = "NewScene") noexcept; // this function acts as the constructor

		ECS::OEECSRegistry& GetRegistry() noexcept { return *m_Registry; }
		const ECS::OEECSRegistry& GetRegistry() const noexcept { return *m_Registry; }

		void DeleteScene() noexcept; 
		bool IsCreated() const noexcept { return m_Created; }
		void SetSceneName(const std::string& name) { m_OESceneName = name; }
		void UpdateSystems(ECS::OEECSRegistry& registry, float dt);
		
		OrionRenderer::ORCamera* GetCurrentCamera() const noexcept { return m_Camera; }

	private:
		std::string m_OESceneName;
		Scope<ECS::OEECSRegistry> m_Registry; // each scene owns each ECS registrry
		bool m_Created;

		OrionRenderer::ORCamera* m_Camera = nullptr;
	};
} 