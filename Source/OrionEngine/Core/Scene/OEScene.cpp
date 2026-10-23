#include "OEScene.h"
#include "OESceneSerializer.h"

#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/ECS/OESystem.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	OEScene::OEScene() = default;
	OEScene::~OEScene() = default;

	void OEScene::InitScene(const std::string& name) noexcept
	{
		m_OESceneName = name;
		m_Registry = CreateScope<ECS::OEECSRegistry>();
		m_Serializer = CreateScope<OESceneSerializer>(this);
		m_Created = true;
	}

	void OEScene::DeleteScene() noexcept
	{
		OE_CORE_ASSERT(m_Registry, "Registry is null!");
		m_Registry->DeleteRegistry();
		m_Created = false;
	}

	void OEScene::SerializeScene()
	{
		m_Serializer->SerializeScene();
	}

	void OEScene::DeserializeScene(const std::filesystem::path& scenePath)
	{
		m_Serializer->DeserializeScene(scenePath);
	}

	void OEScene::CreateProjectDir(const std::string& projName)
	{
		m_Serializer->CreateOEProjectDirectory(projName);
	}
}