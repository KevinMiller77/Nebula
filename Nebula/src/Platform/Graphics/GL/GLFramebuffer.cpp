#include "GLFramebuffer.h"

#include <Nebula_pch.h>
#include <Utils/Logging.h>
#include <glad/glad.h>

#include <ext/imgui/examples/imgui_impl_opengl3.h>
namespace Nebula
{
    static const uint32 C_MaxFrameBufferSize = 8192;

    GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec)
        :   Spec(spec)
    {
		NEB_PROFILE_FUNCTION();
        if (!Spec.SwapChainTarget)
		{
			Resize(spec.Width, spec.Height, true);
			if (Spec.Samples > 1)
			{
				if (IMMultisampleIDs.find(ColorAttachment) == IMMultisampleIDs.end())
				{
					IMMultisampleIDs[ColorAttachment] = spec.Samples;
				}
			}
		}
	}
    GLFramebuffer::~GLFramebuffer()
	{
		NEB_PROFILE_FUNCTION();
        glDeleteFramebuffers(1, &ID);
        glDeleteTextures(1, &ColorAttachment);
        glDeleteTextures(1, &DepthAttachment);
	}

    void GLFramebuffer::Bind()
	{
		if (Spec.SwapChainTarget)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, Spec.Width, Spec.Height);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, ID);
			glViewport(0, 0, Spec.Width, Spec.Height);
		}
    }
    void GLFramebuffer::Unbind()
	{
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void GLFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		NEB_PROFILE_FUNCTION();
        if (!forceRecreate && (Spec.Width == width && Spec.Height == height))
			return;

		Spec.Width = width;
		Spec.Height = height;
		
		if (ID)
		{
			glDeleteFramebuffers(1, &ID);
			glDeleteTextures(1, &ColorAttachment);
			glDeleteTextures(1, &DepthAttachment);
		}

		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

		bool multisample = Spec.Samples > 1;
		if (multisample)
		{
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &ColorAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ColorAttachment);

			glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Spec.Samples, GL_RGBA8, Spec.Width, Spec.Height, GL_FALSE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		else
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
			glBindTexture(GL_TEXTURE_2D, ColorAttachment);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Spec.Width, Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);
		}

		if (multisample)
		{
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &DepthAttachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, DepthAttachment);
			glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Spec.Samples, GL_DEPTH24_STENCIL8, Spec.Width, Spec.Height, GL_FALSE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		else
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, DepthAttachment);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Spec.Width, Spec.Height, 0,
				GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
			);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);
		}

		if (multisample)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, ColorAttachment, 0);
		else
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ColorAttachment, 0);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, DepthAttachment, 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERR("Framebuffer is incomplete!\n");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}

	void GLFramebuffer::BindTexture(uint32_t slot) const
	{
		if(Spec.SwapChainTarget)
			return;
		
		glBindTextureUnit(slot, ColorAttachment);
	}

}