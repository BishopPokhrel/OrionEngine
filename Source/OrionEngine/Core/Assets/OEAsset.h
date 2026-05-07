#pragma once

#include "OEAssetType.h"
#include <string>

namespace OrionEngine::Assets
{
	class OEAsset
	{
	public:

		virtual  ~OEAsset() = 0;

		virtual OEAssetType GetType() const noexcept = 0;
		virtual const std::string& GetName() const noexcept { return m_Name; }

	protected:

		std::string m_Name;
	};
}