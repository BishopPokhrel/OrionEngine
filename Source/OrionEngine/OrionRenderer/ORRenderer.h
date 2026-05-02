#pragma once

#include <glm/glm.hpp>

#include "ORGraphicsAPI.h"
#include "ORRenderCommand.h"
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORIndexBuffer.h"
#include "ORVertexBuffer.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORRenderer
		{
		public:

			static void Init(ORGraphicsAPI API) noexcept
			{
				SetGraphicsAPI(API);
				ORRenderCommand::ORInit();
			}

			static void Shutdown() noexcept
			{

			}

			static void SetViewport(int x, int y, int width, int height) noexcept
			{
				ORRenderCommand::SetViewport(x, y, width, height);
			}

			static void BeginScene(const glm::mat4& viewProjection) noexcept
			{
				s_SceneData.ViewProjectionMatrix = viewProjection;
			}

			static void EndScene() noexcept
			{

			}

			static void Submit(const Ref<ORShader>& shader, const Ref<ORVertexArray>& vertexArray, const glm::mat4& transform) noexcept
			{
				shader->Bind();
				shader->SetMat4("u_ViewProjection", s_SceneData.ViewProjectionMatrix);
				shader->SetMat4("u_Transform", transform);

				vertexArray->Bind();
				
				OE_CORE_ASSERT(vertexArray->GetIndexBuffer(), "VertexArray is missing IndexBuffer!");
				uint32_t indexCount = vertexArray->GetIndexBuffer()->GetCount();
				ORRenderCommand::DrawIndexed(indexCount);
			}

			static void SetGraphicsAPI(ORGraphicsAPI API) noexcept
			{
				s_GraphicsAPI = API;
			}

			static ORGraphicsAPI GetGraphicsAPI() noexcept { return s_GraphicsAPI; }

		private:

			struct SceneData
			{
				glm::mat4 ViewProjectionMatrix;
			};
		
			static SceneData s_SceneData;

			static ORGraphicsAPI s_GraphicsAPI;
		};
	}
}