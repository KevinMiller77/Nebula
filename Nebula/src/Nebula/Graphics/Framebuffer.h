#pragma once

namespace Nebula
{
    struct FramebufferSpecification
	{
		unsigned int Width = 0, Height = 0;
		// FramebufferFormat Format = 
		unsigned int Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(unsigned int width, unsigned int height) = 0;

		virtual unsigned int GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Framebuffer* Create(const FramebufferSpecification& spec);
	};
}