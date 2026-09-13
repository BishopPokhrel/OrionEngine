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
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, -0.0f
		};

		Ref<OpenGLVertexBuffer> VBO = CreateRef<OpenGLVertexBuffer>(vertices, sizeof(vertices));

		ORVertexBufferLayout layout;
		layout.Push<glm::vec3>("a_Position");
		
		VBO->SetLayout(layout);

		s_DefaultVertexArray->AddVertexBuffer(VBO);

		// Index Buffer

		const uint32_t indices[] = {
			0, 1, 2
		};

		Ref<OpenGLIndexBuffer> IBO = CreateRef<OpenGLIndexBuffer>(indices, sizeof(indices));
		s_DefaultVertexArray->SetIndexBuffer(IBO);
		
	}

	const Ref<ORShader>& ORDefaultRendererResources::GetDefaultShader() noexcept
	{
		return s_DefaultShader; 
	}

	const Ref<ORVertexArray>& ORDefaultRendererResources::GetDefaultVertexArray() noexcept
	{
		return s_DefaultVertexArray;
	}

	void ORDefaultRendererResources::Shutdown() noexcept
	{
		s_DefaultShader.reset();
		s_DefaultVertexArray.reset();
	}
}