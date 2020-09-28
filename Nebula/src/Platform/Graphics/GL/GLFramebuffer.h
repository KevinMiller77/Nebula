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

        void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32 width, uint32 height) override;

		virtual uint32 GetColorAttachmentRendererID() const override { return ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return Spec; }
    private:
        uint32 ID;
        uint32 ColorAttachment = 0;
        uint32 DepthAttachment = 0;
      
        FramebufferSpecification Spec;
    };
}