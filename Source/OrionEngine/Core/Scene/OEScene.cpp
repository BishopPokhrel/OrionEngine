#include "OEScene.h"
#include <OrionEngine/Core/OEExceptions.h>

namespace OrionEngine
{
	bool OEScene::CreateOEScene(const std::string& name) 
	{	
		if (m_OEScenes.size() == 0)
		{
			OESceneData newOESceneData;
			newOESceneData.name = name;
			m_OEScenes.push_back(newOESceneData);
			return true;
		}
		else
			throw OEInvalidNumberOfSceneException(name, GetOESceneSize(), " Failed to create scene with name", name, ".");
	}

	bool OEScene::DeleteOEScene(int index = 0) 
	{
		if (index < 0 || index >= static_cast<int>(m_OEScenes.size()))
			return false;

		if (m_OEScenes.empty())
			throw OEInvalidNumberOfSceneException("No scenes exist.", GetOESceneSize());
	
		m_OEScenes.clear(); // Since there is only one scene
		return true;
	}
}