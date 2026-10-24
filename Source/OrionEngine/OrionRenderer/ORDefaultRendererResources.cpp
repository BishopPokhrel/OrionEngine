#include "ORDefaultRendererResources.h"
#include "OROpenGLShaderAPI.h"
#include "ORVertexBufferLayout.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace OrionEngine::OrionRenderer
{
	void ORDefaultRendererResources::Initialize() noexcept
	{
		s_DefaultShader = CreateRef<OROpenGLShaderAPI>();
		s_DefaultShader->CompileAndLinkThroughShaderVariables(
			VertexShader
			{
				R"(
					#version 330 core

					layout(location = 0) in vec3 a_Position;

					uniform mat4 u_ViewProjection;
					uniform mat4 u_Transform;

					void main()
					{
						gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
					}

                )"
			},

			FragmentShader
			{
				R"(
					#version 330 core
					
					out vec4 o_Color;
					
					void main() 
					{
						o_Color = vec4(0.8, 0.8, 0.8, 1.0);
					}
				)"
			}
		);

		s_DefaultVertexArray = CreateRef<OpenGLVertexArray>();

		// Vertex Buffer

		const float vertices[] = {
			// Front face (Z+)
			-0.5f, -0.5f,  0.5f, // 0
			 0.5f, -0.5f,  0.5f, // 1
			 0.5f,  0.5f,  0.5f, // 2
			-0.5f,  0.5f,  0.5f, // 3

			// Back face (Z-)
			-0.5f, -0.5f, -0.5f, // 4
			 0.5f, -0.5f, -0.5f, // 5
			 0.5f,  0.5f, -0.5f, // 6
			-0.5f,  0.5f, -0.5f  // 7
		};

		Ref<OpenGLVertexBuffer> VBO = CreateRef<OpenGLVertexBuffer>(vertices, sizeof(vertices));

		ORVertexBufferLayout layout;
		layout.Push<glm::vec3>("a_Position");

		VBO->SetLayout(layout);
		s_DefaultVertexArray->AddVertexBuffer(VBO);

		// Index Buffer

		const uint32_t indices[] = {
			// Front face
			0, 1, 2, 2, 3, 0,

			// Back face
			5, 4, 7, 7, 6, 5,

			// Left face
			4, 0, 3, 3, 7, 4,

			// Right face
			1, 5, 6, 6, 2, 1,

			// Top face
			3, 2, 6, 6, 7, 3,

			// Bottom face
			4, 5, 1, 1, 0, 4
		};

		Ref<OpenGLIndexBuffer> IBO = CreateRef<OpenGLIndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
		s_DefaultVertexArray->SetIndexBuffer(IBO);

		s_DefaultMesh = CreateRef<ORMesh>(s_DefaultVertexArray);
	}

	const Ref<ORShader>& ORDefaultRendererResources::GetDefaultShader() noexcept
	{
		return s_DefaultShader; 
	}

	const Ref<ORVertexArray>& ORDefaultRendererResources::GetDefaultVertexArray() noexcept
	{
		return s_DefaultVertexArray;
	}

	const Ref<ORMesh>& ORDefaultRendererResources::GetDefaultMesh() noexcept
	{
		return s_DefaultMesh;
	}

	void ORDefaultRendererResources::Shutdown() noexcept
	{
		s_DefaultShader.reset();
		s_DefaultVertexArray.reset();
		s_DefaultMesh.reset();
	}
}