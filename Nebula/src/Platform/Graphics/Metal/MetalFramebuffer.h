#pragma once

#include <Graphics/Framebuffer.h>

namespace MTK {
    class View;
}

namespace MTL {
    class RenderPassColorAttachmentDescriptor;
    class RenderPassDepthAttachmentDescriptor;
    class RenderPassStencilAttachmentDescriptor;
    class RenderPassDescriptor;
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
        MTK::View* m_ContentView;

        std::vector<MTL::RenderPassColorAttachmentDescriptor*> m_ColorAttachments;
        MTL::RenderPassDepthAttachmentDescriptor* m_DepthAttachment;
        MTL::RenderPassStencilAttachmentDescriptor* m_StencilAttachment;

        MTL::RenderPassDescriptor* m_RenderPassDescriptor;
    };
}
