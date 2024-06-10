#include "MetalRendererAPI.h"

#include <AppKit/NSView.hpp>
#include <AppKit/NSWindow.hpp>
#include <Foundation/NSTypes.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Platform/OS/MacOS/MacWindow.h>
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


        auto s_Context = MetalRendererAPI::s_Context;

        Prepare();
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

        s_Context->m_BoundView->setClearColor(
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

        s_Context->m_FrameRenderColorAttatchment = s_Context->m_RenderPassDescriptor->colorAttachments()->object(0);
        s_Context->m_FrameRenderColorAttatchment->setTexture(nullptr);
        s_Context->m_FrameRenderColorAttatchment->setLoadAction(MTL::LoadActionClear);
        s_Context->m_FrameRenderColorAttatchment->setClearColor(s_Context->m_ClearColor);
        s_Context->m_FrameRenderColorAttatchment->setStoreAction(MTL::StoreActionStore);
    }

    void MetalRendererAPI::Clear(Vec4f color)
    {
        SetClearColor(color);
        Clear();
    }

    void MetalRendererAPI::Prepare() {
        if (s_Context->m_RenderCommandEncoder) {
            // Prepare happened before a commit, nothing was intended to be drawn
            // Cleanup and prepare for the next frame
            s_Context->m_RenderCommandEncoder->endEncoding();
            s_Context->m_RenderCommandEncoder = nullptr;

            for (int i = s_Context->m_CommandBuffer->retainCount(); i > 0; i--) {
                s_Context->m_CommandBuffer->release();
            }
            s_Context->m_CommandBuffer = nullptr;
        }

        
        if (s_Context->m_NextView) {
            s_Context->m_BoundView = s_Context->m_NextView;
            s_Context->m_NextView = nullptr;
        }

        if (s_Context->m_BoundView == nullptr) {
            LOG_ERR("[Metal][RendererAPI] Attempted to prepare without a bound view [MetalRendererAPI::Prepare]")
            abort();
        }

        MTK::View* view = (MTK::View*)(NS::Window*)s_Context->m_BoundView;
        s_Context->m_CommandBuffer = s_Context->m_CommandQueue->commandBuffer();
        s_Context->m_RenderPassDescriptor = view->currentRenderPassDescriptor();
        s_Context->m_RenderCommandEncoder = s_Context->m_CommandBuffer->renderCommandEncoder(s_Context->m_RenderPassDescriptor);
        s_Context->m_RenderCommandEncoder->setCullMode(MTL::CullMode::CullModeNone);

        s_Context->m_FrameRenderColorAttatchment = s_Context->m_RenderPassDescriptor->colorAttachments()->object(0);
        s_Context->m_FrameRenderColorAttatchment->setLoadAction(MTL::LoadActionClear);
        s_Context->m_FrameRenderColorAttatchment->setClearColor(s_Context->m_ClearColor);
        s_Context->m_FrameRenderColorAttatchment->setStoreAction(MTL::StoreActionStore);

        s_Context->m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        s_Context->m_RenderCommandEncoder->setCullMode(MTL::CullModeNone);

        NS::Size drawableSize = s_Context->m_BoundView->drawableSize();
        SetViewport(0, 0, drawableSize.width, drawableSize.height);

        s_Context->m_RenderCommandEncoder->setViewport(s_Context->m_Viewport);
    }
    
    void MetalRendererAPI::Commit() {
        if (s_Context->m_RenderCommandEncoder == nullptr) {
            LOG_ERR("[Metal][RendererAPI] Attempted to commit without a render command encoder [MetalRendererAPI::Commit")
            return;
        }

        s_Context->m_RenderCommandEncoder->endEncoding();
        s_Context->m_RenderCommandEncoder = nullptr;

        MTK::View* view = (MTK::View*)(NS::Window*)s_Context->m_BoundView;
        if (!view->framebufferOnly()) {
            s_Context->m_CommandBuffer->presentDrawable(view->currentDrawable());
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

}