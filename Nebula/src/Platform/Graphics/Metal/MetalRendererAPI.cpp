#include "MetalRendererAPI.h"

#include <Utils/Logging.h>
#include <Metal/Metal.hpp>

namespace Nebula {

    Ref<MetalContext> MetalRendererAPI::s_Context = nullptr;

    void MetalRendererAPI::Init(Ref<GraphicsContext> context)
    {
        s_Context = CastRef<MetalContext>(context);

        s_Context->m_Device = MTL::CreateSystemDefaultDevice();
        s_Context->m_CommandQueue = s_Context->m_Device->newCommandQueue();

        // Create a Metal shader library
        // Create a Metal render pipeline state
        // Create a Metal depth-stencil state

        s_Context->m_AlphaBlendEnabled = false;
        s_Context->m_DepthTestEnabled = false;
        s_Context->m_MSAAEnabled = false;

        s_Context->m_Viewport = {0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f};
        s_Context->m_ClearColor = MTL::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void MetalRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_Context->m_Viewport = {
            (double)x,
            (double)y,
            (double)width,
            (double)height,
            0.0,
            1.0
        };
    }

    void MetalRendererAPI::SetClearColor(const Vec4f& color)
    {
        s_Context->m_ClearColor.red = color.X;
        s_Context->m_ClearColor.green = color.Y;
        s_Context->m_ClearColor.blue = color.Z;
        s_Context->m_ClearColor.alpha = color.W;
    }

    void MetalRendererAPI::Clear()
    {
        MTL::CommandBuffer* commandBuffer = s_Context->m_CommandQueue->commandBuffer();
        // Create a new render pass descriptor from MTL cpp MTL::
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::renderPassDescriptor();

        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
        colorAttachment->setTexture(nullptr);
        colorAttachment->setLoadAction(MTL::LoadActionClear);
        colorAttachment->setClearColor(s_Context->m_ClearColor);
        colorAttachment->setStoreAction(MTL::StoreActionStore);

        MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
        renderEncoder->setViewport(s_Context->m_Viewport);
        renderEncoder->setRenderPipelineState(s_Context->m_RenderPipelineState);
        renderEncoder->setDepthStencilState(s_Context->m_DepthStencilState);
        renderEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        renderEncoder->setCullMode(MTL::CullModeBack);
        renderEncoder->setVertexBuffer(nullptr, 0, 0);
        renderEncoder->setFragmentTexture(nullptr, 0);
        renderEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, 0, 3, 0, 1);
        renderEncoder->endEncoding();

        commandBuffer->presentDrawable(nullptr);
        commandBuffer->commit();
    }

    void MetalRendererAPI::Clear(Vec4f color)
    {
        SetClearColor(color);
        Clear();
    }

    void MetalRendererAPI::SetAlphaBlend(bool enabled)
    {
        // s_Context->m_AlphaBlendEnabled = enabled;
    }

    void MetalRendererAPI::SetDepthTest(bool enabled)
    {
        s_Context->m_DepthTestEnabled = enabled;
    }

    void MetalRendererAPI::SetMSAA(bool enabled)
    {
        s_Context->m_MSAAEnabled = enabled;
    }

    void MetalRendererAPI::DrawIndexed(const int numIndices, PrimativeType type, bool depthTest)
    {
        MTL::CommandBuffer* commandBuffer = s_Context->m_CommandQueue->commandBuffer();
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::renderPassDescriptor();

        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(1);
        colorAttachment->setTexture(nullptr);
        colorAttachment->setLoadAction(MTL::LoadActionClear);
        colorAttachment->setClearColor(s_Context->m_ClearColor);
        colorAttachment->setStoreAction(MTL::StoreActionStore);

        MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
        renderEncoder->setViewport(s_Context->m_Viewport);
        renderEncoder->setRenderPipelineState(s_Context->m_RenderPipelineState);
        renderEncoder->setDepthStencilState(s_Context->m_DepthStencilState);
        renderEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        renderEncoder->setCullMode(MTL::CullModeBack);
        renderEncoder->setVertexBuffer(nullptr, 0, 0);
        renderEncoder->setFragmentTexture(nullptr, 0);

        switch (type)
        {
            case PrimativeType::TRIANGLES:
                renderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numIndices, MTL::IndexTypeUInt32, nullptr, 0, 3);
                break;

            case PrimativeType::LINES:
                renderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeLine, numIndices, MTL::IndexTypeUInt32, nullptr, 0, 2);
                break;

            default:
                break;
        }

        renderEncoder->endEncoding();

        commandBuffer->presentDrawable(nullptr);
        commandBuffer->commit();
    }

    void MetalRendererAPI::DrawElementsBaseVertex(PrimativeType mode, uint32_t size, PrimativeDataType type, void* indices, uint32_t base)
    {
        MTL::CommandBuffer* commandBuffer = s_Context->m_CommandQueue->commandBuffer();
        // Create a new render pass descriptor from MTL cpp MTL::
        MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::renderPassDescriptor();

        MTL::RenderPassColorAttachmentDescriptor* colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
        colorAttachment->setTexture(nullptr);
        colorAttachment->setLoadAction(MTL::LoadActionClear);
        colorAttachment->setClearColor(s_Context->m_ClearColor);
        colorAttachment->setStoreAction(MTL::StoreActionStore);

        MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
        renderEncoder->setViewport(s_Context->m_Viewport);
        renderEncoder->setRenderPipelineState(s_Context->m_RenderPipelineState);
        renderEncoder->setDepthStencilState(s_Context->m_DepthStencilState);
        renderEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        renderEncoder->setCullMode(MTL::CullModeBack);
        renderEncoder->setVertexBuffer(nullptr, 0, 0);
        renderEncoder->setFragmentTexture(nullptr, 0);

        MTL::IndexType indexType;
        switch (type)
        {
            case PrimativeDataType::UNSIGNED_BYTE:
                // TODO: Figure out how to get 8 bit boys
                indexType = MTL::IndexTypeUInt16;
                break;
            case PrimativeDataType::UNSIGNED_SHORT:
                indexType = MTL::IndexTypeUInt16;
                break;
            case PrimativeDataType::UNSIGNED_INT:
                indexType = MTL::IndexTypeUInt32;
                break;
            default:
                indexType = MTL::IndexTypeUInt16;
                break;
        }

        MTL::Buffer* indexBuffer = s_Context->m_Device->newBuffer(indices, size, MTL::ResourceCPUCacheModeDefaultCache);
        switch (mode)
        {
            case PrimativeType::TRIANGLES:
                renderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, size, indexType, indexBuffer, base, 3);
                break;

            case PrimativeType::LINES:
                renderEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeLine, size, indexType, indexBuffer, base, 2);
                break;

            default:
                break;
        }

        renderEncoder->endEncoding();

        commandBuffer->presentDrawable(nullptr);
        commandBuffer->commit();
    }

}