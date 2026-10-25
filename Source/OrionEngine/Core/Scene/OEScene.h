#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>

namespace OrionEngine::ECS
{
	class OEECSRegistry; 
}

namespace OrionEngine
{
	class OESceneSerializer;

	class OEScene
	{
	public:
		
		OEScene();
		~OEScene();

		void InitScene(const std::string& name = "NewScene") noexcept; // this function acts as the constructor

		ECS::OEECSRegistry& GetRegistry() noexcept { return *m_Registry; }
		const ECS::OEECSRegistry& GetRegistry() const noexcept { return *m_Registry; }

		void DeleteScene() noexcept; 
		bool IsCreated() const noexcept { return m_Created; }
		void SetSceneName(const std::string& name) { m_OESceneName = name; }
		
		OrionRenderer::ORCamera& GetCurrentCamera() noexcept { return m_Camera; }
		const OrionRenderer::ORCamera& GetCurrentCamera() const noexcept { return m_Camera; }

		std::string GetSceneName() const noexcept { return m_OESceneName; }

		void SerializeScene();
		void DeserializeScene(const std::filesystem::path& scenePath);
		void CreateProjectDir(const std::string& projName);

	private:
		std::string m_OESceneName;
		Scope<ECS::OEECSRegistry> m_Registry; // each scene owns each ECS registrry
		Scope<OESceneSerializer> m_Serializer;
		bool m_Created;

		OrionRenderer::ORCamera m_Camera = OrionRenderer::ORCamera(-8.0f, 8.0f, 4.5f, -4.5f);
	};
}  