#include "ORDefaultRendererResources.h"
#include "OROpenGLShaderAPI.h"
#include "ORVertexBufferLayout.h"

#include "OROpenGLVertexArrayAPI.h"
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
					layout(location = 2) in vec2 a_TexCoord;


					uniform mat4 u_ViewProjection;
					uniform mat4 u_Transform;

					out vec3 v_FragPosition;
					out vec3 v_Normal;
					out vec2 v_TexCoord;

					void main()
					{
						vec4 worldPosition = u_Transform * vec4(a_Position, 1.0);

						v_FragPosition = worldPosition.xyz;
						v_TexCoord = a_TexCoord;

						mat3 normalMatrix = transpose(inverse(mat3(u_Transform)));
						v_Normal = normalize(normalMatrix * a_Normal);

						gl_Position = u_ViewProjection * worldPosition;
					}
				)"
			},

			FragmentShader
			{
				R"(

					#version 330 core

					out vec4 o_Color;

					in vec3 v_FragPosition;
					in vec3 v_Normal;
					in vec2 v_TexCoord;

					uniform sampler2D u_AlbedoTexture;
					uniform bool u_HasAlbedoTexture;

					// Camera
					uniform vec3 u_CameraPosition;

					// Directional Light
					uniform vec3 u_LightDirection;
					uniform vec3 u_LightColor;
					uniform float u_LightIntensity;

					// Material
					uniform vec4 u_AlbedoColor;
					uniform float u_Metallic;
					uniform float u_Roughness;
					uniform float u_AmbientOcclusion;

					const float PI = 3.14159265359;

					// --------------------------------------------------
					// GGX Normal Distribution
					// --------------------------------------------------

					float DistributionGGX(vec3 normal, vec3 halfway, float roughness)
					{
						float a = roughness * roughness;
						float a2 = a * a;

						float NdotH = max(dot(normal, halfway), 0.0);
						float NdotH2 = NdotH * NdotH;

						float denominator = NdotH2 * (a2 - 1.0) + 1.0;

						return a2 / max(PI * denominator * denominator, 0.000001);
					}

					// --------------------------------------------------
					// Schlick-GGX Geometry
					// --------------------------------------------------

					float GeometrySchlickGGX(float NdotV, float roughness)
					{
						float r = roughness + 1.0;
						float k = (r * r) / 8.0;

						return NdotV / max(NdotV * (1.0 - k) + k, 0.000001);
					}

					// --------------------------------------------------
					// Smith Geometry
					// --------------------------------------------------

					float GeometrySmith(
						vec3 normal,
						vec3 viewDirection,
						vec3 lightDirection,
						float roughness)
					{
						float NdotV = max(dot(normal, viewDirection), 0.0);
						float NdotL = max(dot(normal, lightDirection), 0.0);

						float geometryView = GeometrySchlickGGX(NdotV, roughness);
						float geometryLight = GeometrySchlickGGX(NdotL, roughness);

						return geometryView * geometryLight;
					}

					// --------------------------------------------------
					// Schlick Fresnel
					// --------------------------------------------------

					vec3 FresnelSchlick(float cosTheta, vec3 F0)
					{
						return F0 + (1.0 - F0) *
							pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
					}

					// --------------------------------------------------
					// Main
					// --------------------------------------------------

					void main()
					{
						vec4 albedoColor = u_AlbedoColor;

						if (u_HasAlbedoTexture)
							albedoColor *= texture(u_AlbedoTexture, v_TexCoord);

						vec3 albedo = albedoColor.rgb;

						float metallic = clamp(u_Metallic, 0.0, 1.0);
						float roughness = clamp(u_Roughness, 0.04, 1.0);
						float ao = clamp(u_AmbientOcclusion, 0.0, 1.0);

						vec3 normal = normalize(v_Normal);
						vec3 lightDirection = normalize(-u_LightDirection);
						vec3 viewDirection = normalize(u_CameraPosition - v_FragPosition);

						float NdotL = max(dot(normal, lightDirection), 0.0);
						float NdotV = max(dot(normal, viewDirection), 0.0);

						vec3 halfway = normalize(viewDirection + lightDirection);

						vec3 F0 = mix(vec3(0.04), albedo, metallic);

						vec3 F = FresnelSchlick(
							max(dot(halfway, viewDirection), 0.0),
							F0
						);

						float NDF = DistributionGGX(normal, halfway, roughness);

						float G = GeometrySmith(
							normal,
							viewDirection,
							lightDirection,
							roughness
						);

						vec3 numerator = NDF * G * F;
						float denominator = max(4.0 * NdotV * NdotL, 0.0001);

						vec3 specular = numerator / denominator;

						vec3 kS = F;
						vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

						vec3 diffuse = kD * albedo / PI;

						vec3 directLighting =
							(diffuse + specular) *
							u_LightColor *
							u_LightIntensity *
							NdotL;

						vec3 ambient = vec3(0.03) * albedo * ao;

						vec3 color = ambient + directLighting;

						color = color / (color + vec3(1.0));
						color = pow(color, vec3(1.0 / 2.2));

						 o_Color = vec4(color, albedoColor.a);
					}
				)"
			}
		);


		s_DefaultVertexArray = CreateRef<OROpenGLVertexArrayAPI>();

		// Vertex Buffer

		const float vertices[] = {
			// Front face (+Z)
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

			// Back face (-Z)
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, 0.0f, 1.0f,

			 // Left face (-X)
			 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f, 1.0f,

			 // Right face (+X)
			  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			  // Top face (+Y)
			  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			   0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			   0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			  // Bottom face (-Y)
			  -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
			   0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f, 1.0f, 0.0f,
			   0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f, 1.0f, 1.0f,
			  -0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f, 0.0f, 1.0f
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
		layout.Push<glm::vec2>("a_TexCoord");

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
		s_DefaultMaterial = CreateRef<AssetManagement::OEMaterial>();
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

	const Ref<AssetManagement::OEMaterial>& ORDefaultRendererResources::GetDefaultMaterial() noexcept
	{
		return s_DefaultMaterial;
	}

	void ORDefaultRendererResources::Shutdown() noexcept
	{
		s_DefaultShader.reset();
		s_DefaultVertexArray.reset();
		s_DefaultMesh.reset();
		s_DefaultMaterial.reset();
	}
}