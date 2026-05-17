#pragma once

#include <glm/glm.hpp>

#include "ORGraphicsAPI.h"
#include "ORRenderCommand.h"
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORIndexBuffer.h"
#include "ORVertexBuffer.h"
#include "ORSceneData.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORRenderer
		{
		public:

			void Init(ORGraphicsAPI API) noexcept
			{
				SetGraphicsAPI(API);
				ORRenderCommand::ORInit();
			}

			void Shutdown() noexcept
			{

			}

			void SetViewport(int x, int y, int width, int height) noexcept
			{
				ORRenderCommand::SetViewport(x, y, width, height);
			}

			void BeginScene(const glm::mat4& viewProjection) noexcept
			{
				m_SceneData.ViewProjectionMatrix = viewProjection;
			}

			void EndScene() noexcept
			{

			}

			void Submit(const Ref<ORShader>& shader, const Ref<ORVertexArray>& vertexArray, const glm::mat4& transform)
			{
				shader->Bind();
				shader->SetMat4("u_ViewProjection", m_SceneData.ViewProjectionMatrix);
				shader->SetMat4("u_Transform", transform);

				vertexArray->Bind();
				
				OE_CORE_ASSERT(vertexArray->GetIndexBuffer(), "VertexArray is missing IndexBuffer!");
				uint32_t indexCount = vertexArray->GetIndexBuffer()->GetCount();
				ORRenderCommand::DrawIndexed(indexCount);
			}

			void SetGraphicsAPI(ORGraphicsAPI API) noexcept
			{
				m_GraphicsAPI = API;
			}

			ORGraphicsAPI GetGraphicsAPI() const noexcept { return m_GraphicsAPI; }

		private:
		
			SceneData m_SceneData;
			ORGraphicsAPI m_GraphicsAPI;
		};
	}
}