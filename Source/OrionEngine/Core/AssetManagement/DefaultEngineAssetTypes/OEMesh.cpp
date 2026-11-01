#include "OEMesh.h"

#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine::AssetManagement
{
	OEMesh::OEMesh(const Ref<OrionRenderer::ORMesh>& mesh)
	{
		OE_CORE_ASSERT(mesh, "Mesh is null!");
		m_Mesh = mesh;
	}
}