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

		virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) override;

        virtual void BindTexture(uint32_t slot = 0) const override;

		virtual uint32_t GetRendererID() const override { return ID; };
		virtual uint32_t GetDepthAttachmentRendererID() const override { return DepthAttachment; };
		virtual uint32 GetColorAttachmentRendererID() const override { return ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return Spec; }
    private:
        uint32 ID;
        uint32 ColorAttachment = 0;
        uint32 DepthAttachment = 0;
      
        FramebufferSpecification Spec;
    };
}