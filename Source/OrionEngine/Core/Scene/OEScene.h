#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>
#include <OrionEngine/OrionRenderer/OEDirectionalLight.h>

#include <glm/glm.hpp>

namespace OrionEngine::ECS
{
	class OEECSRegistry; 
}

namespace OrionEngine::AssetManagement
{
	class OEAssetManager;
}

namespace OrionEngine
{
	class OESceneSerializer;

	class OEScene
	{
	public:
		
		explicit OEScene(AssetManagement::OEAssetManager* assetManager);
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
		OESceneSerializer* GetSerializer() const noexcept { return m_Serializer.get(); }
		AssetManagement::OEAssetManager* GetAssetManager() const noexcept { return m_AssetManager; }

	private:
		std::string m_OESceneName;
		AssetManagement::OEAssetManager* m_AssetManager = nullptr; // The scene does not own the asset manager; OEngine does

		Scope<ECS::OEECSRegistry> m_Registry; // each scene owns each ECS registrry
		Scope<OESceneSerializer> m_Serializer;
		bool m_Created;

		OrionRenderer::ORCamera m_Camera = OrionRenderer::ORCamera(-8.0f, 8.0f, 4.5f, -4.5f);
		OrionRenderer::OEDirectionalLight m_DirectionalLight;
	};
}  