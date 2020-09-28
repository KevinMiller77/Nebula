#pragma once
#include <Core/NebulaCommon.h>

namespace Nebula
{
    struct FramebufferSpecification
	{
		uint32 Width = 0, Height = 0;
		// FramebufferFormat Format = 
		uint32 Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;

		virtual uint32 GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Framebuffer* Create(const FramebufferSpecification& spec);
	};
}