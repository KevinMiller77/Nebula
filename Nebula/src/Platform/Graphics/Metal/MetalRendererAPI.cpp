#include "MetalRendererAPI.h"

#include <AppKit/NSView.hpp>
#include <AppKit/NSWindow.hpp>
#include <Foundation/NSTypes.hpp>
#include <Graphics/RendererConfig.h>
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPass.hpp>
#include <Metal/MTLRenderPipeline.hpp>
#include <Metal/MTLTexture.hpp>
#include <Platform/OS/MacOS/MacWindow.h>
#include <QuartzCore/CAMetalDrawable.hpp>
#include <Utils/Logging.h>
#include <Metal/Metal.hpp>
#include <objc/NSObjCRuntime.h>

namespace Nebula {

    Ref<MetalContext> MetalRendererAPI::s_Context = nullptr;

    MTL::Device* MetalRendererAPI::GetDevice() {
        if (s_Context == nullptr) {
            LOG_ERR("[Metal][RendererAPI] Attempted to get device without a context [MetalRendererAPI::GetDevice]")
            abort();
        }

        return s_Context->m_Device;
    }

    void MetalRendererAPI::Init(Ref<GraphicsContext> context)
    {
        s_Context = CastRef<MetalContext>(context);
        
        s_Context->m_Device = MTL::CreateSystemDefaultDevice();
        s_Context->m_CommandQueue = s_Context->m_Device->newCommandQueue();

        s_Context->m_CommandBuffer = s_Context->m_CommandQueue->commandBuffer();
        s_Context->m_CommandBuffer->retain();

        // Create a Metal shader library
        // Create a Metal render pipeline state
        // Create a Metal depth-stencil state

        s_Context->m_AlphaBlendEnabled = false;
        s_Context->m_DepthTestEnabled = false;
        s_Context->m_MSAAEnabled = false;
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

        s_Context->m_MainView->setClearColor(
            MTL::ClearColor(
                color.X,
                color.Y,
                color.Z,
                color.W
            )
        );
    }

    void MetalRendererAPI::Clear()
    {
        if (s_Context->m_RenderCommandEncoder == nullptr) {
            Prepare();
        }

        auto frameRenderColorAttatchment = s_Context->m_RenderPassDescriptor->colorAttachments()->object(0);
        // frameRenderColorAttatchment->setTexture(nullptr);
        frameRenderColorAttatchment->setLoadAction(MTL::LoadActionClear);
        frameRenderColorAttatchment->setClearColor(s_Context->m_ClearColor);
        frameRenderColorAttatchment->setStoreAction(MTL::StoreActionStore);
    }

    void MetalRendererAPI::Clear(Vec4f color)
    {
        SetClearColor(color);
        Clear();
    }

    void MetalRendererAPI::Prepare() {
        switch (s_Context->m_CommandBuffer->status()) {
            case(MTL::CommandBufferStatusNotEnqueued): {
                // Command buffer has never been used. Nothing to do
                break;
            }
            case(MTL::CommandBufferStatusEnqueued): {
                // Command buffer has been enqueued but not scheduled, we should commit
                Commit();
                Prepare();
                return;
            }
            case(MTL::CommandBufferStatusScheduled): {
                // Buffer is scheduled to be executed, wait until it is completed
                // then call this method again to catch any errors
                s_Context->m_CommandBuffer->waitUntilCompleted();
                return Prepare();
            }
            case(MTL::CommandBufferStatusCompleted): { 
                s_Context->m_CommandBuffer->release();

                s_Context->m_CommandBuffer = s_Context->m_CommandQueue->commandBuffer();
                s_Context->m_CommandBuffer->retain();
                break;
            }
            case(MTL::CommandBufferStatusCommitted): {
                // We should never reach this state, but if we do, log it and abort
                LOG_ERR("[Metal][RendererAPI] Command buffer status committed [MetalRendererAPI::Prepare]")
                abort();
                break;
            }
            case(MTL::CommandBufferStatusError): {
                // Log the error
                LOG_ERR("[Metal][RendererAPI] Command buffer status error: %s", s_Context->m_CommandBuffer->error()->description()->utf8String());
                abort();
                break;
            }
        }

        if (s_Context->m_RenderPassDescriptor == nullptr || s_Context->m_RenderPassDescriptor == s_Context->m_MainRenderPassDescriptor) {
            // LOG_ERR("[Metal][RendererAPI] Attempted to prepare without a render pass descriptor [MetalRendererAPI::Prepare]")
            // abort();

            s_Context->m_MainRenderPassDescriptor = s_Context->m_MainView->currentRenderPassDescriptor();
            s_Context->m_RenderPassDescriptor = s_Context->m_MainRenderPassDescriptor;

            NS::Size drawableSize = s_Context->m_MainView->drawableSize();
            RendererConfig::SetViewport(0, 0, drawableSize.width, drawableSize.height);

            // auto frameRenderColorAttachment = s_Context->m_RenderPassDescriptor->colorAttachments()->object(0);
            // frameRenderColorAttachment->setLoadAction(MTL::LoadActionClear);
            // frameRenderColorAttachment->setClearColor(s_Context->m_ClearColor);
            // frameRenderColorAttachment->setStoreAction(MTL::StoreActionStore);
        }

        s_Context->m_RenderCommandEncoder = s_Context->m_CommandBuffer->renderCommandEncoder(s_Context->m_RenderPassDescriptor);
        s_Context->m_RenderCommandEncoder->retain();
    }
    
    void MetalRendererAPI::Commit() {
        if (s_Context->m_RenderCommandEncoder == nullptr) {
            LOG_ERR("[Metal][RendererAPI] Attempted to commit without a render command encoder [MetalRendererAPI::Commit")
            return;
        }

        s_Context->m_RenderCommandEncoder->endEncoding();
        if (s_Context->m_RenderCommandEncoder) {
            s_Context->m_RenderCommandEncoder->release();
        }
        s_Context->m_RenderCommandEncoder = nullptr;


        CA::MetalDrawable* drawable = s_Context->m_MainView->currentDrawable();
        if (drawable) {
            s_Context->m_CommandBuffer->presentDrawable(drawable);
        }
        s_Context->m_CommandBuffer->commit();
        s_Context->m_CommandBuffer->waitUntilScheduled();
    }

    void MetalRendererAPI::SetAlphaBlend(bool enabled)
    {
        s_Context->m_AlphaBlendEnabled = enabled;
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
        if (s_Context->m_RenderCommandEncoder == nullptr) {
            Prepare();
        }

        s_Context->m_RenderCommandEncoder->setVertexBuffer(s_Context->m_VertexBuffer, 0, 0);

        switch (type)
        {
            case PrimativeType::TRIANGLES:
                s_Context->m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numIndices, MTL::IndexTypeUInt32, s_Context->m_IndexBuffer, 0, 3);
                break;

            case PrimativeType::LINES:
                s_Context->m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeLine, numIndices, MTL::IndexTypeUInt32, s_Context->m_IndexBuffer, 0, 2);
                break;

            default:
                break;
        }
    }

    void MetalRendererAPI::DrawElementsBaseVertex(PrimativeType mode, uint32_t size, PrimativeDataType type, void* indices, uint32_t base)
    {
        if (s_Context->m_RenderCommandEncoder == nullptr) {
            Prepare();
        }

        s_Context->m_RenderCommandEncoder->setVertexBuffer(s_Context->m_VertexBuffer, 0, 0);
        
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
        
        switch (mode)
        {
            case PrimativeType::TRIANGLES:
                s_Context->m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, size, indexType, s_Context->m_IndexBuffer, base, 3);
                break;

            case PrimativeType::LINES:
                s_Context->m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeLine, size, indexType, s_Context->m_IndexBuffer, base, 2);
                break;

            default:
                break;
        }
    }


    void MetalRendererAPI::BindRenderPass(MTL::RenderPassDescriptor* renderPass, MTL::Viewport viewport) {
        if (s_Context->m_RenderCommandEncoder) {
            LOG_ERR("[MetalRendererAPI][BindRenderPass] Attempted to bind a render pass before committing the previous!\n");
            abort();
        }

        s_Context->m_RenderPassDescriptor = renderPass;
        s_Context->m_Viewport = viewport;
    }

    void MetalRendererAPI::UnbindRenderPass() {
        if (s_Context->m_RenderCommandEncoder) {
            RendererConfig::Commit();
        }


        s_Context->m_RenderPassDescriptor = nullptr;
    }

    void MetalRendererAPI::BindVertexBuffer(MTL::Buffer* vertexBuffer) {
        s_Context->m_VertexBuffer = vertexBuffer;
    }

    void MetalRendererAPI::BindIndexBuffer(MTL::Buffer* indexBuffer) {
        s_Context->m_IndexBuffer = indexBuffer;
    }

    void MetalRendererAPI::BindShader(MTL::RenderPipelineState* shader) {
        s_Context->m_RenderCommandEncoder->setRenderPipelineState(shader);
    }

    void MetalRendererAPI::BindShaderVertexBuffer(MTL::Buffer* buffer, uint32_t offset, uint32_t index) {
        s_Context->m_RenderCommandEncoder->setVertexBuffer(buffer, offset, index);
    }

    void MetalRendererAPI::BindShaderFragmentBuffer(MTL::Buffer* buffer, uint32_t offset, uint32_t index) {
        s_Context->m_RenderCommandEncoder->setFragmentBuffer(buffer, offset, index);
    }

    void MetalRendererAPI::BindTexture2D(MTL::Texture* texture, uint32_t slot) {
        s_Context->m_RenderCommandEncoder->setFragmentTexture(texture, slot);
    }

    void MetalRendererAPI::BindShaderFragmentTexture(MTL::Texture* texture, uint32_t slot) {
        s_Context->m_RenderCommandEncoder->setFragmentTexture(texture, slot);
    }

}
