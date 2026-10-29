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
					layout(location = 1) in vec3 a_Normal;

					uniform mat4 u_ViewProjection;
					uniform mat4 u_Transform;

					out vec3 v_Normal;

					void main()
					{
						mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
						v_Normal = normalize(normalMatrix * a_Normal);

						gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
					}                
				)"
			},

			FragmentShader
			{
				R"(
					#version 330 core

					in vec3 v_Normal;

					out vec4 o_Color;

					uniform vec3 u_LightDirection;
					uniform vec3 u_LightColor;
					uniform float u_LightIntensity;

					void main()
					{
						vec3 normal = normalize(v_Normal);
						vec3 lightDirection = normalize(-u_LightDirection);

						float diffuse = max(dot(normal, lightDirection), 0.0);

						vec3 ambient = vec3(0.1);
						vec3 lighting = ambient + u_LightColor * diffuse * u_LightIntensity;

						o_Color = vec4(vec3(0.8) * lighting, 1.0);
					}				
				)"
			}
		);

		s_DefaultVertexArray = CreateRef<OpenGLVertexArray>();

		// Vertex Buffer

		const float vertices[] = {
			// Front face (+Z)
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

			// Back face (-Z)
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f,

			 // Left face (-X)
			 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			 -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
			 -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
			 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

			 // Right face (+X)
			  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
			  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

			  // Top face (+Y)
			  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			   0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			   0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

			  // Bottom face (-Y)
			  -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,
			   0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f,
			   0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f,
			  -0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f
		};

		s_DefaultShader->Bind();

		s_DefaultShader->SetVec3("u_LightDirection", glm::vec3(-0.5f, -1.0f, -0.5f));
		s_DefaultShader->SetVec3("u_LightColor", glm::vec3(1.0f));
		s_DefaultShader->SetFloat("u_LightIntensity", 1.0f);

		s_DefaultShader->Unbind();

		Ref<OpenGLVertexBuffer> VBO = CreateRef<OpenGLVertexBuffer>(vertices, sizeof(vertices));

		ORVertexBufferLayout layout;
		layout.Push<glm::vec3>("a_Position");
		layout.Push<glm::vec3>("a_Normal");

		VBO->SetLayout(layout);
		s_DefaultVertexArray->AddVertexBuffer(VBO);

		// Index Buffer

		const uint32_t indices[] = {
			// Front face (+Z)
			0, 1, 2, 2, 3, 0,

			// Back face (-Z)
			4, 5, 6, 6, 7, 4,

			// Left face (-X)
			8, 9, 10, 10, 11, 8,

			// Right face (+X)
			12, 13, 14, 14, 15, 12,

			// Top face (+Y)
			16, 17, 18, 18, 19, 16,

			// Bottom face (-Y)
			20, 21, 22, 22, 23, 20
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