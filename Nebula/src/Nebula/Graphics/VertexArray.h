#pragma once

#include "Buffer.h"

namespace Nebula
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
        virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;

        virtual const std::vector<VertexBuffer*> GetVertexBuffers() const = 0;
        virtual const IndexBuffer* GetIndexBuffer() const = 0;

        static VertexArray* Create();
    };
}