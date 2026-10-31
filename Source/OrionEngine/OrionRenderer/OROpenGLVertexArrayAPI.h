#pragma once

#include "ORVertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace OrionEngine::OrionRenderer
{
    class OROpenGLVertexArrayAPI : public ORVertexArray
    {
    public:

        OROpenGLVertexArrayAPI() noexcept
            : m_VertexArray(CreateRef<OpenGLVertexArray>())
        { }

        void Bind() const noexcept override
        {
            m_VertexArray->Bind();
        }

        void Unbind() const noexcept override
        {
            m_VertexArray->Unbind();
        }

        void AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept override
        {
            m_VertexArray->AddVertexBuffer(VBO);
        }

        void SetIndexBuffer(const Ref<ORIndexBuffer>& IBO) noexcept override
        {
            m_VertexArray->SetIndexBuffer(IBO);
        }

        const Ref<ORIndexBuffer>& GetIndexBuffer() const noexcept override
        {
            return m_VertexArray->GetIndexBuffer();
        }

    private:

        Ref<OpenGLVertexArray> m_VertexArray;
    };
}