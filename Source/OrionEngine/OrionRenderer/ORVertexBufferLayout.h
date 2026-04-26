#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace OrionEngine::OrionRenderer
{
	enum class ORShaderDataType
	{
		None = 0,

		Float, 
		Float2, 
		Float3, 
		Float4,
		
		Int, 
		Int2, 
		Int3, 
		Int4,
		
		Mat3, 
		Mat4,
		Bool
	};

	inline uint32_t ShaderDataTypeSize(ORShaderDataType type)
	{
		switch (type)
		{
			case ORShaderDataType::Float:  return 4;
			case ORShaderDataType::Float2: return 4 * 2;
			case ORShaderDataType::Float3: return 4 * 3;
			case ORShaderDataType::Float4: return 4 * 4;

			case ORShaderDataType::Mat3:   return 4 * 3 * 3;
			case ORShaderDataType::Mat4:   return 4 * 4 * 4;

			case ORShaderDataType::Int:    return 4;
			case ORShaderDataType::Int2:   return 4 * 2;
			case ORShaderDataType::Int3:   return 4 * 3;
			case ORShaderDataType::Int4:   return 4 * 4;

			case ORShaderDataType::Bool:   return 1;
			default: return 0;
		}
	}

	inline uint32_t ShaderDataTypeComponentCount(ORShaderDataType type)
	{
		switch (type)
		{
			case ORShaderDataType::Float:  return 1;
			case ORShaderDataType::Float2: return 2;
			case ORShaderDataType::Float3: return 3;
			case ORShaderDataType::Float4: return 4;

			case ORShaderDataType::Int:    return 1;
			case ORShaderDataType::Int2:   return 2;
			case ORShaderDataType::Int3:   return 3;
			case ORShaderDataType::Int4:   return 4;

			case ORShaderDataType::Mat3:   return 3 * 3;
			case ORShaderDataType::Mat4:   return 4 * 4;

			case ORShaderDataType::Bool:   return 1;
			default: return 0;
		}
	}

	struct VertexBufferElement
	{
		std::string Name;
		ORShaderDataType Type;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		bool Normalized = false;
	};

	class ORVertexBufferLayout
	{
	public:
		ORVertexBufferLayout() = default;

		const std::vector<VertexBufferElement>& GetElements() const noexcept { return m_Elements; }
		uint32_t GetStride() const noexcept { return m_Stride; }

		template<typename T>
		void Push(const std::string& name) = delete;

	private:
		void AddElement(const std::string& name, ORShaderDataType type) noexcept
		{
			VertexBufferElement element;
			element.Name = name;
			element.Type = type;
			element.Size = ShaderDataTypeSize(type);
			element.Offset = m_Stride;
			element.Normalized = false;

			m_Elements.push_back(element);
			m_Stride += element.Size;
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	// TEMPLATE SPECIALIZATION
	template<>
	inline void ORVertexBufferLayout::Push<float>(const std::string& name) noexcept
	{
		AddElement(name, ORShaderDataType::Float);
	}

	template<>
	inline void ORVertexBufferLayout::Push<glm::vec2>(const std::string& name) noexcept
	{
		AddElement(name, ORShaderDataType::Float2);
	}

	template<>
	inline void ORVertexBufferLayout::Push<glm::vec3>(const std::string& name) noexcept
	{
		AddElement(name, ORShaderDataType::Float3);
	}

	template<>
	inline void ORVertexBufferLayout::Push<glm::vec4>(const std::string& name) noexcept
	{
		AddElement(name, ORShaderDataType::Float4);
	}

	template<>
	inline void ORVertexBufferLayout::Push<int>(const std::string& name) noexcept
	{
		AddElement(name, ORShaderDataType::Int);
	}
}