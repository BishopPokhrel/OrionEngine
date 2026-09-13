#pragma once

#include <OrionEngine/OrionRenderer/ORShader.h>
#include <OrionEngine/OrionRenderer/ORVertexArray.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionRenderer
{
	class ORDefaultRendererResources
	{
	public:

		static void Initialize() noexcept;

		static const Ref<ORShader>& GetDefaultShader() noexcept;
		static const Ref<ORVertexArray>& GetDefaultVertexArray() noexcept;

		static void Shutdown() noexcept;

	private:

		inline static Ref<ORShader> s_DefaultShader;
		inline static Ref<ORVertexArray> s_DefaultVertexArray;
	};
}