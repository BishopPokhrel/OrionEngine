#pragma once

#include "ORVertexArray.h"

#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORMesh
		{
		public:

			explicit ORMesh(ORVertexArray* vao, uint32_t indexCount)
				: m_VAO(vao), m_IndexCount(indexCount)
			{}

			void Bind() const noexcept
			{
				OE_CORE_ASSERT(m_VAO, "Vertex Array is null!");
				m_VAO->Bind();
			}
			uint32_t GetIndexCount() const noexcept { return m_IndexCount; }

		private:

			ORVertexArray* m_VAO = nullptr;
			uint32_t m_IndexCount = 0;
		};
	}
}