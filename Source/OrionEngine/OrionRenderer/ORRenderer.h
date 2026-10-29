#pragma once

#include <glm/glm.hpp>

#include "ORGraphicsAPI.h"
#include "ORRenderCommand.h"
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORIndexBuffer.h"
#include "ORVertexBuffer.h"
#include "ORSceneData.h"
#include "ORMesh.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OELogging.h>

// for diagnosis only
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		static void CheckSubmitError(const char* location)
		{
			GLenum error = GL_NO_ERROR;

			while ((error = glGetError()) != GL_NO_ERROR)
				ORION_ENGINE_ERROR("OpenGL error at ", location, ": ", static_cast<unsigned int>(error));
		}

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

			void Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const glm::mat4& transform)
			{
				OE_CORE_ASSERT(shader, "Submit received a null shader!");
				OE_CORE_ASSERT(mesh, "Submit received a null mesh!");

				shader->Bind();
				shader->SetMat4("u_ViewProjection", m_SceneData.ViewProjectionMatrix);
				shader->SetMat4("u_Transform", transform);
				shader->SetVec3("u_LightDirection", glm::vec3(-0.5f, -1.0f, -0.5f));
				shader->SetVec3("u_LightColor", glm::vec3(1.0f));
				shader->SetFloat("u_LightIntensity", 1.0f);

				mesh->Bind();

				ORRenderCommand::DrawIndexed(mesh->GetIndexCount());
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