#include "MetalFramebuffer.h"

#include <Graphics/Framebuffer.h>
#include <Graphics/RenderPass.h>

#include <AppKit/AppKit.hpp>
#include <Graphics/RendererConfig.h>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <Platform/Graphics/Metal/MetalContext.h>
#include <cstddef>

namespace Nebula {
    MetalFramebuffer::MetalFramebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {

        Resize(spec.Width, spec.Height);

        NS::Rect frame = {
        {   .x = 0, 
                .y =  0 
            }, 
            {   
                .width = (NS::Float)m_Specification.Width, 
                .height = (NS::Float)m_Specification.Height
            } 
        };

        m_ContentView = MTK::View::alloc()->init( 
            frame, 
            MetalRendererAPI::s_Context->m_Device 
        );

        m_ContentView->setAutoresizeDrawable(false);
        m_ContentView->setPresentsWithTransaction(false);
    }

    void MetalFramebuffer::Invalidate() {
        
    }

    void MetalFramebuffer::Bind() {
        auto s_Context = MetalRendererAPI::s_Context;
        MetalRendererAPI::s_Context->m_NextView = m_ContentView;
        
        RendererConfig::Prepare();
	};

    void MetalFramebuffer::Unbind() {
        RendererConfig::Commit();

        auto s_Context = MetalRendererAPI::s_Context;
        MetalRendererAPI::s_Context->m_NextView = s_Context->m_DefaultContentView;
        RendererConfig::Prepare();
	};

    void MetalFramebuffer::Resize(uint32 width, uint32 height) {
		m_Specification.Width = width;
		m_Specification.Height = height;

        NS::Rect frame = {
            .origin = {   .x = 0, 
                .y =  0 
            }, 
            .size = {   
                .width = (NS::Float)m_Specification.Width, 
                .height = (NS::Float)m_Specification.Height
            } 
        };

        m_ContentView->setDrawableSize(frame.size);
        // Invalidate();
	};

    int MetalFramebuffer::ReadPixel(uint32 colorAttachmentID, int x, int y) {

        // int* pixelByte = (int*)malloc(sizeof(int));
        // m_ContentView->currentDrawable()->texture()->getBytes(
        //     pixelByte, 
        //     m_ContentView->currentDrawable()->texture()->bufferBytesPerRow(),
        //     { .x = x, .y = y}, 
        //     0
        // );

        return 0;
	};
    void MetalFramebuffer::ClearTextureAttachment(uint32 attachmentID, const int value) {
            
	};
    void MetalFramebuffer::ClearTextureAttachment(uint32 attachmentID, const float value) {

	};

    void* MetalFramebuffer::GetRendererID() const {
        return nullptr;
	};
    void* MetalFramebuffer::GetDepthAttachmentRendererID() const {
        return nullptr;
	};
    std::vector<void*> MetalFramebuffer::GetColorAttachmentsRendererID() const {
        return {m_ContentView->currentDrawable()->texture()};
	};
    const FramebufferSpecification& MetalFramebuffer::GetSpecification() const {
        return m_Specification;
	};
}