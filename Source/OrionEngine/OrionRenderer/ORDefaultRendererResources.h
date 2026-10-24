#pragma once

#include <OrionEngine/Core/OERef.h>

#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORMesh.h"

namespace OrionEngine::OrionRenderer
{
	class ORDefaultRendererResources
	{
	public:

		static void Initialize() noexcept;

		static const Ref<ORShader>& GetDefaultShader() noexcept;
		static const Ref<ORVertexArray>& GetDefaultVertexArray() noexcept;
		static const Ref<ORMesh>& GetDefaultMesh() noexcept; 

		static void Shutdown() noexcept;

	private:

		inline static Ref<ORShader> s_DefaultShader;
		inline static Ref<ORVertexArray> s_DefaultVertexArray;
		inline static Ref<ORMesh> s_DefaultMesh;
	};
}