#pragma once

#include <Graphics/Framebuffer.h>
#include <Metal/MTLRenderCommandEncoder.hpp>

namespace MTK {
    class View;
}

namespace MTL {
    class RenderPipelineColorAttachmentDescriptor;
    class RenderPassColorAttachmentDescriptor;
    class RenderPassDepthAttachmentDescriptor;
    class RenderPassStencilAttachmentDescriptor;
    class RenderPassDescriptor;
    class Texture;
}

namespace Nebula {
    class MetalFramebuffer : public Framebuffer
    {
    public:
        MetalFramebuffer(const FramebufferSpecification& spec);
        virtual ~MetalFramebuffer() {};

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32 width, uint32 height) override;
        virtual int ReadPixel(uint32 colorAttachmentID, int x, int y) override;
        
        void ClearTextureAttachment(uint32 attachmentID, const int value) override;
        void ClearTextureAttachment(uint32 attachmentID, const float value) override;
		
        void* GetRendererID() const override;
		void* GetDepthAttachmentRendererID() const override;
		std::vector<void*> GetColorAttachmentsRendererID() const override;
		
        const FramebufferSpecification& GetSpecification() const override;

    private:
        void Invalidate();

        FramebufferSpecification m_Specification;

        std::vector<MTL::Texture*> m_ColorAttachments;
        MTL::Texture* m_DepthAttachment;
        MTL::Viewport m_Viewport;

        MTL::RenderPassDescriptor* m_RenderPassDescriptor;
    };
}
