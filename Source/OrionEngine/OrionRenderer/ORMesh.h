#pragma once

#include "ORVertexArray.h"
#include "ORIndexBuffer.h"

#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORMesh
		{
		public:

			explicit ORMesh(const Ref<ORVertexArray>& VAO)
			{
				OE_CORE_ASSERT(VAO, "VAO is null!");
				OE_CORE_ASSERT(VAO->GetIndexBuffer(), "Current mesh does not have index buffer!");
				
				m_VAO = VAO; 
			}

			void Bind()
			{
				OE_CORE_ASSERT(m_VAO, "VAO is null!");
				m_VAO->Bind();
			}

			uint32_t GetIndexCount()
			{
				OE_CORE_ASSERT(m_VAO, "VAO is null!");
				OE_CORE_ASSERT(m_VAO->GetIndexBuffer(), "Index Buffer is null!");

				return m_VAO->GetIndexBuffer()->GetCount();
			}

			const Ref<ORVertexArray>& GetVertexArray() const noexcept { return m_VAO;  }

		private:

			Ref<ORVertexArray> m_VAO;
		};
	}
}