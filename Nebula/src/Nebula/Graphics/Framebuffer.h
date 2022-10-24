#pragma once

#include <vector>

#include <Core/NebulaCommon.h>
#include <Core/Ref.h>
#include <Math/math.h>

namespace Nebula{
    enum class FramebufferTextureFormat 
    {
        None = 0,

        //Col
        RGBA8,
        RED_INT,

        //Depth-Stencil
        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8
    };
    struct FramebufferTextureSpecification 
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format)
            : TextureFormat(format) {}

        FramebufferTextureFormat TextureFormat;
    };

    struct FramebufferAttachmentSpecification
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
            : TextureAttachments(attachments) {}

        std::vector<FramebufferTextureSpecification> TextureAttachments;
    };

    struct FramebufferSpecification
	{
		uint32 Width = 0, Height = 0;
		// FramebufferFormat Format = 
		uint32 Samples = 1;
        FramebufferAttachmentSpecification Attachments;
        Vec4f ClearColor = {0.1f, 0.1f, 0.1f, 1.0f};

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
        virtual int ReadPixel(uint32 attachmentID, int x, int y) = 0;
        virtual void ClearTextureAttachment(uint32 attachmentID, const int value) = 0;
        virtual void ClearTextureAttachment(uint32 attachmentID, const float value) = 0;

		virtual uint32_t GetRendererID() const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;
		virtual std::vector<uint32_t> GetColorAttachmentsRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}