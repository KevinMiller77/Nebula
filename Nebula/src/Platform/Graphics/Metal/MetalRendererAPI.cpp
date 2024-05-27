#include "MetalRendererAPI.h"

#include <AppKit/NSView.hpp>
#include <AppKit/NSWindow.hpp>
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

        EndFrame();
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


        s_Context->m_View->setClearColor(
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
        s_Context->m_RenderCommandEncoder->endEncoding();

        MTK::View* view = (MTK::View*)((NS::Window*)s_Context->m_Window)->contentView();
        s_Context->m_FrameCommandBuffer->presentDrawable(view->currentDrawable());
        s_Context->m_FrameCommandBuffer->commit();

        EndFrame();
    }

    void MetalRendererAPI::Clear(Vec4f color)
    {
        SetClearColor(color);
        Clear();
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

    void MetalRendererAPI::EndFrame()
    {
        s_Context->m_FrameCommandBuffer = s_Context->m_CommandQueue->commandBuffer();
        s_Context->m_RenderPassDescriptor = s_Context->m_View->currentRenderPassDescriptor();
        s_Context->m_RenderCommandEncoder = s_Context->m_FrameCommandBuffer->renderCommandEncoder(s_Context->m_RenderPassDescriptor);

        s_Context->m_FrameRenderColorAttatchment = s_Context->m_RenderPassDescriptor->colorAttachments()->object(0);
        // s_Context->m_FrameRenderColorAttatchment->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        s_Context->m_FrameRenderColorAttatchment->setLoadAction(MTL::LoadActionClear);
        s_Context->m_FrameRenderColorAttatchment->setClearColor(s_Context->m_ClearColor);
        s_Context->m_FrameRenderColorAttatchment->setStoreAction(MTL::StoreActionStore);
        s_Context->m_FrameRenderColorAttatchment->setTexture(nullptr);

        // s_Context->m_FrameCommandBuffer = s_Context->m_CommandQueue->commandBuffer();
        // s_Context->m_RenderPassDescriptor = s_Context->m_View->currentRenderPassDescriptor();

        
        // s_Context->m_FrameRenderCommandEncoder = s_Context->m_FrameCommandBuffer->renderCommandEncoder(s_Context->m_RenderPassDescriptor );

        // s_Context->m_FrameRenderCommandEncoder->setViewport(s_Context->m_Viewport);
        // s_Context->m_FrameRenderCommandEncoder->setDepthStencilState(s_Context->m_DepthStencilState);
        // s_Context->m_FrameRenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        // s_Context->m_FrameRenderCommandEncoder->setCullMode(MTL::CullModeBack);
        // s_Context->m_FrameRenderCommandEncoder->setFragmentTexture(nullptr, 0);
    }

    void MetalRendererAPI::DrawIndexed(const int numIndices, PrimativeType type, bool depthTest)
    {
        s_Context->m_RenderCommandEncoder->setViewport(s_Context->m_Viewport);
        s_Context->m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        s_Context->m_RenderCommandEncoder->setCullMode(MTL::CullModeBack);
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

        s_Context->m_RenderCommandEncoder->endEncoding();

        MTK::View* view = (MTK::View*)((NS::Window*)s_Context->m_Window)->contentView();
        s_Context->m_FrameCommandBuffer->presentDrawable(view->currentDrawable());
        s_Context->m_FrameCommandBuffer->commit();
        s_Context->m_FrameCommandBuffer->waitUntilScheduled();

        EndFrame();
    }

    void MetalRendererAPI::DrawElementsBaseVertex(PrimativeType mode, uint32_t size, PrimativeDataType type, void* indices, uint32_t base)
    {
        s_Context->m_RenderCommandEncoder->setViewport(s_Context->m_Viewport);
        s_Context->m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingCounterClockwise);
        s_Context->m_RenderCommandEncoder->setCullMode(MTL::CullModeBack);
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

        s_Context->m_RenderCommandEncoder->endEncoding();


        s_Context->m_FrameCommandBuffer->commit();
        s_Context->m_FrameCommandBuffer->waitUntilScheduled();

        EndFrame();
    }

}