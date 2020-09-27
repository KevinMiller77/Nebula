#pragma once

#include <Graphics/Framebuffer.h>

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

		virtual void Resize(unsigned int width, unsigned int height) override;

		virtual unsigned int GetColorAttachmentRendererID() const override { return ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return Spec; }
    private:
        unsigned int ID;
        unsigned int ColorAttachment = 0;
        unsigned int DepthAttachment = 0;
      
        FramebufferSpecification Spec;
    };
}