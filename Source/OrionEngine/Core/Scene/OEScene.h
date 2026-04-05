#pragma once

#include <string>
#include <vector>

namespace OrionEngine
{
	class OEScene
	{
	public:

		bool CreateOEScene(const std::string& name);
		bool DeleteOEScene(int index = 0);

	private:

		struct OESceneData
		{
			std::string name;
			// Later, add a vector of entities
		};

		std::vector<OESceneData> m_OEScenes;
		size_t GetOESceneSize() { return m_OEScenes.size(); }
	};
}