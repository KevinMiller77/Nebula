#pragma once

#include <Graphics/VertexArray.h>
#include <Core/NebulaCommon.h>
#include <Platform/Graphics/Metal/MetalShader.h>

namespace MTL {
    class VertexDescriptor;
}

namespace Nebula{
    class MetalVertexArray : public VertexArray
    {
    public:
        MetalVertexArray();
        virtual ~MetalVertexArray() override;

        virtual void Bind() const override;
        virtual void Unbind() const override {};

        virtual void AddVertexBuffer(Ref<VertexBuffer>) override;
        virtual void SetIndexBuffer(Ref<IndexBuffer>) override;

        virtual const std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
        virtual const Ref<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
        
    private:
        uint32 m_IndexPosition = 0;
        MTL::VertexDescriptor* m_VertexDescriptor = nullptr;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;

        friend class MetalShader;
    };
}