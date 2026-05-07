#pragma once

#include <string>
#include <unordered_map>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::Assets
{
	template<typename T>
	class OEAssetManager
	{
	public:

		Ref<T> Load(const std::string& path)
		{
			auto it = m_Assets.find(path);
			if (it != m_Assets.end())
				return it->second;

			Ref<T> asset = CreateRef<T>(path);
			m_Assets[path] = asset;

			return asset;
		}

		Ref<T> Get(const std::string& path)
		{
			auto it = m_Assets.find(path);
			if (it != m_Assets.end())
				return it->second;

			return nullptr;
		}

		bool Exists(const std::string& path) const
		{
			return m_Assets.find(path) != m_Assets.end();
		}

	private:

		std::unordered_map<std::string, Ref<T>> m_Assets;
	};
}