#include "MetalBuffers.h"

#include <Nebula_pch.h>

namespace Nebula {

    MetalVertexBuffer::MetalVertexBuffer(uint32_t size)
    {
        NEB_PROFILE_FUNCTION();
        buffer = MTL::CreateSystemDefaultDevice()->newBuffer(size, MTL::ResourceStorageModeManaged);
    }

    MetalVertexBuffer::MetalVertexBuffer(float* vertices, uint32_t size)
    {
        NEB_PROFILE_FUNCTION();
        buffer = MTL::CreateSystemDefaultDevice()->newBuffer(vertices, size, MTL::ResourceStorageModeManaged);
    }

    MetalVertexBuffer::~MetalVertexBuffer()
    {
    }

    void MetalVertexBuffer::Bind() const
    {
    }

    void MetalVertexBuffer::Unbind() const
    {
    }

    void MetalVertexBuffer::SetData(const void* data, uint32_t size)
    {
        NEB_PROFILE_FUNCTION();
        memcpy(buffer->contents(), data, size);
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        return CreateRef<MetalVertexBuffer>(size);
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        return CreateRef<MetalVertexBuffer>(vertices, size);
    }

    MetalIndexBuffer::MetalIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        NEB_PROFILE_FUNCTION();
        buffer = MTL::CreateSystemDefaultDevice()->newBuffer(indices, count * sizeof(uint32_t), MTL::ResourceStorageModeManaged);
    }

    MetalIndexBuffer::~MetalIndexBuffer()
    {
    }

    void MetalIndexBuffer::Bind() const
    {
    }

    void MetalIndexBuffer::Unbind() const
    {
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        return CreateRef<MetalIndexBuffer>(indices, count);
    }
}
