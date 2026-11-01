#include "OEMeshLoader.h"

#include "tiny_obj_loader.h"

#include <OrionEngine/Core/OELogging.h>

// Renderer includes
#include <OrionEngine/OrionRenderer/OROpenGLVertexArrayAPI.h>
#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLVertexBuffer.h>
#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLIndexBuffer.h>
#include <OrionEngine/OrionRenderer/ORVertexBufferLayout.h>

namespace OrionEngine::AssetManagement
{
	Ref<OEAsset> OEMeshLoader::LoadAsset(const std::filesystem::path& assetPath)
	{
		if (assetPath.empty())
			return nullptr;

		// Parse the OBJ file using tinyobjloader 
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(assetPath.string()))
		{
			const std::string& error = reader.Error();
			ORION_ENGINE_ERROR("Failed to parse OBJ file at path ", assetPath.string(), " due to error ", error);
			return nullptr;
		}

		const tinyobj::attrib_t& attributes = reader.GetAttrib();
		const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
		const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

		std::vector<OEMeshVertex> vertices;
		std::vector<uint32_t> indices;

        for (const tinyobj::shape_t& shape : shapes)
        {
            size_t indexOffset = 0;

            for (const unsigned char faceVertexCount : shape.mesh.num_face_vertices)
            {
                // We expect triangles because triangulation is enabled.
                if (faceVertexCount != 3)
                    return nullptr;

                // Retrieve the three indices belonging to this triangle.
                const tinyobj::index_t& index0 = shape.mesh.indices[indexOffset];
                const tinyobj::index_t& index1 = shape.mesh.indices[indexOffset + 1];
                const tinyobj::index_t& index2 = shape.mesh.indices[indexOffset + 2];

                // Retrieve vertex positions.
                glm::vec3 position0(
                    attributes.vertices[3 * index0.vertex_index],
                    attributes.vertices[3 * index0.vertex_index + 1],
                    attributes.vertices[3 * index0.vertex_index + 2]
                );

                glm::vec3 position1(
                    attributes.vertices[3 * index1.vertex_index],
                    attributes.vertices[3 * index1.vertex_index + 1],
                    attributes.vertices[3 * index1.vertex_index + 2]
                );

                glm::vec3 position2(
                    attributes.vertices[3 * index2.vertex_index],
                    attributes.vertices[3 * index2.vertex_index + 1],
                    attributes.vertices[3 * index2.vertex_index + 2]
                );

                // Calculate a fallback normal for missing normals.
                glm::vec3 faceNormal = glm::cross(
                    position1 - position0,
                    position2 - position0
                );

                if (glm::length(faceNormal) > 0.000001f)
                    faceNormal = glm::normalize(faceNormal);
                else
                    faceNormal = glm::vec3(0.0f, 1.0f, 0.0f);

                const tinyobj::index_t triangleIndices[] = { index0, index1, index2 };

                for (const tinyobj::index_t& index : triangleIndices)
                {
                    OEMeshVertex vertex;

                    vertex.Position = glm::vec3(
                        attributes.vertices[3 * index.vertex_index],
                        attributes.vertices[3 * index.vertex_index + 1],
                        attributes.vertices[3 * index.vertex_index + 2]
                    );

                    if (index.normal_index >= 0)
                    {
                        vertex.Normal = glm::vec3(
                            attributes.normals[3 * index.normal_index],
                            attributes.normals[3 * index.normal_index + 1],
                            attributes.normals[3 * index.normal_index + 2]
                        );
                    }
                    else
                        vertex.Normal = faceNormal;

                    if (index.texcoord_index >= 0)
                    {
                        vertex.UV = glm::vec2(
                            attributes.texcoords[2 * index.texcoord_index],
                            attributes.texcoords[2 * index.texcoord_index + 1]
                        );
                    }
                    else
                        vertex.UV = glm::vec2(0.0f);

                    indices.push_back(static_cast<uint32_t>(vertices.size()));
                    vertices.push_back(vertex);
                }

                indexOffset += faceVertexCount;
            }
        }

		Ref<OrionRenderer::OROpenGLVertexArrayAPI> VAO = CreateRef<OrionRenderer::OROpenGLVertexArrayAPI>();
		Ref<OrionRenderer::OpenGLVertexBuffer> VBO = CreateRef<OrionRenderer::OpenGLVertexBuffer>(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(OEMeshVertex)));

		OrionRenderer::ORVertexBufferLayout layout;
		layout.Push<glm::vec3>("a_Position");
		layout.Push<glm::vec3>("a_Normal");
		layout.Push<glm::vec2>("a_UV");

		VBO->SetLayout(layout);
		VAO->AddVertexBuffer(VBO);

		Ref<OrionRenderer::OpenGLIndexBuffer> IBO = CreateRef<OrionRenderer::OpenGLIndexBuffer>(indices.data(), static_cast<uint32_t>(indices.size()));

		VAO->SetIndexBuffer(IBO);

		// Construct the mesh
		Ref<OrionRenderer::ORMesh> mesh = CreateRef<OrionRenderer::ORMesh>(VAO);
		Ref<OEAsset> asset = CreateRef<OEMesh>(mesh);

		return asset;
	}
}