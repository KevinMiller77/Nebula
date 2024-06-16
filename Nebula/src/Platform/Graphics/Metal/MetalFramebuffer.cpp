#include "MetalFramebuffer.h"

#include <Graphics/Framebuffer.h>
#include <Graphics/RenderPass.h>

#include <AppKit/AppKit.hpp>
#include <Graphics/RendererConfig.h>
#include <Metal/MTLPixelFormat.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <Platform/Graphics/Metal/MetalContext.h>
#include <cstddef>
#include <objc/NSObjCRuntime.h>

namespace Nebula {
    MetalFramebuffer::MetalFramebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        Invalidate();
    }

    void MetalFramebuffer::Invalidate() {
        m_Viewport = {
            .originX = 0,
            .originY = 0,
            .width = (double)m_Specification.Width,
            .height = (double)m_Specification.Height,
            .znear = 0.0f, // TODO: Add to spec
            .zfar = 1.0f   // TODO: Add to spec
        };
        
        // Invalidate all color attachments
        for (auto& colorAttachment : m_ColorAttachments) {
            colorAttachment->release();
        }
        m_ColorAttachments.clear();

        // Invalidate depth attachment
        if (m_DepthAttachment) {
            m_DepthAttachment->release();
        }

        if (m_RenderPassDescriptor) {
            m_RenderPassDescriptor->release();
        }

        // Create a new render pass descriptor
        m_RenderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
        m_RenderPassDescriptor->retain();
        for (auto att: m_Specification.Attachments.TextureAttachments) {
            switch (att.TextureFormat) {
                case FramebufferTextureFormat::RGBA8:
                {
                    MTL::TextureDescriptor* colorTexture = MTL::TextureDescriptor::texture2DDescriptor(
                        MTL::PixelFormatBGRA8Unorm,
                        m_Specification.Width,
                        m_Specification.Height,
                        false
                    );
                    colorTexture->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
                    colorTexture->setStorageMode(MTL::StorageModeShared);
                    colorTexture->setTextureType(MTL::TextureType2D);
                    colorTexture->setSampleCount(m_Specification.Samples);


                    MTL::Texture* colorAttachment = MetalRendererAPI::GetDevice()->newTexture(colorTexture);

                    MTL::RenderPassColorAttachmentDescriptor* colorAttachmentDescriptor = MTL::RenderPassColorAttachmentDescriptor::alloc()->init();
                    colorAttachmentDescriptor->setTexture(colorAttachment);
                    colorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
                    colorAttachmentDescriptor->setStoreAction(MTL::StoreActionStore);
                    colorAttachmentDescriptor->setClearColor(
                        {
                            m_Specification.ClearColor.X, 
                            m_Specification.ClearColor.Y, 
                            m_Specification.ClearColor.Z, 
                            m_Specification.ClearColor.W
                        }
                    );

                    m_RenderPassDescriptor->colorAttachments()->setObject(colorAttachmentDescriptor, m_ColorAttachments.size());

                    colorAttachment->retain();
                    m_ColorAttachments.push_back(colorAttachment);
                    break;
                }
                case FramebufferTextureFormat::RED_INT: {
                    MTL::TextureDescriptor* colorTexture = MTL::TextureDescriptor::texture2DDescriptor(
                        MTL::PixelFormatR32Sint,
                        m_Specification.Width,
                        m_Specification.Height,
                        false
                    );
                    colorTexture->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
                    colorTexture->setStorageMode(MTL::StorageModeShared);
                    colorTexture->setTextureType(MTL::TextureType2D);
                    colorTexture->setSampleCount(m_Specification.Samples);

                    MTL::Texture* colorAttachment = MetalRendererAPI::GetDevice()->newTexture(colorTexture);
                    MTL::RenderPassColorAttachmentDescriptor* colorAttachmentDescriptor = MTL::RenderPassColorAttachmentDescriptor::alloc()->init();
                    colorAttachmentDescriptor->setTexture(colorAttachment);
                    colorAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
                    colorAttachmentDescriptor->setStoreAction(MTL::StoreActionStore);
                    colorAttachmentDescriptor->setClearColor(
                        {
                            m_Specification.ClearColor.X,
                            m_Specification.ClearColor.Y,
                            m_Specification.ClearColor.Z,
                            m_Specification.ClearColor.W 
                        }
                    );

                    m_RenderPassDescriptor->colorAttachments()->setObject(colorAttachmentDescriptor, m_ColorAttachments.size());

                    colorAttachment->retain();
                    m_ColorAttachments.push_back(colorAttachment);
                    break;
                }
                // case FramebufferTextureFormat::DEPTH24STENCIL8: {
                //     MTL::TextureDescriptor* depthTexture = MTL::TextureDescriptor::texture2DDescriptor(
                //         MTL::PixelFormatDepth24Unorm_Stencil8,
                //         m_Specification.Width,
                //         m_Specification.Height,
                //         false
                //     );
                //     depthTexture->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
                //     depthTexture->setStorageMode(MTL::StorageModePrivate);
                //     depthTexture->setTextureType(MTL::TextureType2D);
                //     depthTexture->setSampleCount(m_Specification.Samples);

                //     m_DepthAttachment = MetalRendererAPI::GetDevice()->newTexture(depthTexture);

                //     MTL::RenderPassDepthAttachmentDescriptor* depthAttachmentDescriptor = MTL::RenderPassDepthAttachmentDescriptor::alloc()->init();
                //     depthAttachmentDescriptor->setTexture(m_DepthAttachment);
                //     depthAttachmentDescriptor->setLoadAction(MTL::LoadActionClear);
                //     depthAttachmentDescriptor->setStoreAction(MTL::StoreActionStore);
                //     depthAttachmentDescriptor->setClearDepth(1.0f);

                //     m_RenderPassDescriptor->setDepthAttachment(depthAttachmentDescriptor);
                //     break;
                // }
                case FramebufferTextureFormat::None: {
                    LOG_ERR("[Metal][Framebuffer] FramebufferTextureFormat::NONE is currently not supported");
                    break;
                }
            }
        }
    }

    void MetalFramebuffer::Bind() {
        auto s_Context = MetalRendererAPI::s_Context;

        MetalRendererAPI::BindRenderPass(m_RenderPassDescriptor, m_Viewport);
        RendererConfig::Prepare();
        RendererConfig::Clear(m_Specification.ClearColor);
	};

    void MetalFramebuffer::Unbind() {
        RendererConfig::Commit();
        MetalRendererAPI::UnbindRenderPass();
	};

    void MetalFramebuffer::Resize(uint32 width, uint32 height) {
        m_Specification.Width = width;
        m_Specification.Height = height;

		Invalidate();
	};

    int MetalFramebuffer::ReadPixel(uint32 colorAttachmentID, int x, int y) {
        if (m_ColorAttachments.size() < colorAttachmentID) {
            LOG_ERR("[Metal][Framebuffer] Color attachment ID is out of bounds");
            return -1;
        }

        MTL::Texture* colorAttachment = m_ColorAttachments[colorAttachmentID];
        if (!colorAttachment) {
            LOG_ERR("[Metal][Framebuffer] Color attachment is null");
            return -1;
        }

        MTL::Region region = MTL::Region::Make2D(x, y, 1, 1);

        int pixel = -1;
        colorAttachment->getBytes(&pixel, colorAttachment->bufferBytesPerRow(), region, 0);
        return pixel;
	};
    void MetalFramebuffer::ClearTextureAttachment(uint32 attachmentID, const int value) {
        if (m_ColorAttachments.size() < attachmentID) {
            LOG_ERR("[Metal][Framebuffer] Color attachment ID is out of bounds");
            return;
        }

        MTL::Texture* colorAttachment = m_ColorAttachments[attachmentID];
        if (!colorAttachment) {
            LOG_ERR("[Metal][Framebuffer] Color attachment is null");
            return;
        }

        MTL::Region region = MTL::Region::Make2D(0, 0, colorAttachment->width(), colorAttachment->width());
        colorAttachment->replaceRegion(region, 0, &value, sizeof(int));
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
        std::vector<void*> out = {};
        for (auto colorAttachment : m_ColorAttachments) {
            out.push_back(colorAttachment);
        }
        return out;
	};
    const FramebufferSpecification& MetalFramebuffer::GetSpecification() const {
        return m_Specification;
	};
}
