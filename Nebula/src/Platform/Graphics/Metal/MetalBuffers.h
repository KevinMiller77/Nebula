#pragma once

#include <Graphics/Buffer.h>
#include <Metal/Metal.hpp>

namespace Nebula {
    class MetalVertexBuffer : public VertexBuffer {
    public:
        MetalVertexBuffer(uint32_t size);
        MetalVertexBuffer(float* vertices, uint32_t size);
        virtual ~MetalVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual const BufferLayout& GetLayout() const override { return layout; }
        virtual void SetLayout(const BufferLayout& layout) override { this->layout = layout; }

        static Ref<VertexBuffer> Create(uint32_t size);
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

    private:
        MTL::Buffer* buffer;
        BufferLayout layout;
    };

    class MetalIndexBuffer : public IndexBuffer {
    public:
        MetalIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~MetalIndexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const override { return m_Count; }

        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

    private:
        MTL::Buffer* buffer;
        uint32_t m_Count;
    };
}
