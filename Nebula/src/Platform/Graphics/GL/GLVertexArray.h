#pragma once

#include <Graphics/VertexArray.h>
#include <Core/NebulaCommon.h>

namespace Nebula
{
    class GLVertexArray : public VertexArray
    {
    public:
        GLVertexArray();
        virtual ~GLVertexArray() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
        virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return vertexBuffers; }
        virtual const Ref<IndexBuffer> GetIndexBuffer() const override { return indexBuffer; }
    private:
        uint32 ID;
        uint32 indexPosition = 0;
        std::vector<Ref<VertexBuffer>> vertexBuffers;
        Ref<IndexBuffer> indexBuffer;

    };
}