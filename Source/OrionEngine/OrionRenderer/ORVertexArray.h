#pragma once

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionRenderer
{
    class ORVertexBuffer;
    class ORIndexBuffer;

    class ORVertexArray
    {
    public:
        virtual ~ORVertexArray() noexcept = default;

        virtual void Bind() const noexcept = 0;
        virtual void Unbind() const noexcept = 0;

        virtual void AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept = 0;
        virtual void SetIndexBuffer(const Ref<ORIndexBuffer>& IBO) noexcept = 0;

        virtual const Ref<ORIndexBuffer>& GetIndexBuffer() const noexcept = 0;
    };
}