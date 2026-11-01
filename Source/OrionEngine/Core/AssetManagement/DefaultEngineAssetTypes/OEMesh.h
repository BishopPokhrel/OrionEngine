#pragma once

#include <OrionEngine/Core/AssetManagement/OEAsset.h>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/ORMesh.h>

namespace OrionEngine::AssetManagement
{
	class OEMesh : public OEAsset
	{
	public:

		explicit OEMesh(const Ref<OrionRenderer::ORMesh>& mesh);
		~OEMesh() = default;

		const Ref<OrionRenderer::ORMesh>& GetMesh() const noexcept { return m_Mesh; }
		
	private:

		Ref<OrionRenderer::ORMesh> m_Mesh;
	};
}
