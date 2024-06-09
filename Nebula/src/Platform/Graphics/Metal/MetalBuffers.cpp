#include "MetalBuffers.h"

#include <Nebula_pch.h>
#include "MetalContext.h"

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
        MetalRendererAPI::s_Context->m_VertexBuffer = buffer;
    }

    void MetalVertexBuffer::Unbind() const
    {

    }

    void MetalVertexBuffer::SetData(const void* data, uint32_t size)
    {
        NEB_PROFILE_FUNCTION();
        memcpy(buffer->contents(), data, size);
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
        MetalRendererAPI::s_Context->m_IndexBuffer = buffer;
    }

    void MetalIndexBuffer::Unbind() const
    {
    }

}
