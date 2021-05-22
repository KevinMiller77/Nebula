#pragma once

#include <Graphics/Framebuffer.h>
#include <Core/NebulaCommon.h>

namespace Nebula
{
    class GLFramebuffer : public Framebuffer
    {
    public:
        GLFramebuffer(const FramebufferSpecification& spec);
        virtual ~GLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32 width, uint32 height) override;
        void Invalidate();

        // virtual void BindTexture(uint32_t slot = 0) const override;

		virtual uint32_t GetRendererID() const override { return ID; };
		virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; };
		virtual std::vector<uint32> GetColorAttachmentsRendererID() const override { return m_ColorAttachments; }

		virtual const FramebufferSpecification& GetSpecification() const override { return Spec; }
    private:
        uint32 ID;
      
        FramebufferSpecification Spec;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
        FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::Depth; 

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment;
    };
}