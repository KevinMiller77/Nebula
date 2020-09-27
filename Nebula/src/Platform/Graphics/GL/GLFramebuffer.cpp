#include "GLFramebuffer.h"

#include <Utils/Logging.h>
#include <glad/glad.h>

namespace Nebula
{
    static const unsigned int C_MaxFrameBufferSize = 8192;

    GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec)
        :   Spec(spec)
    {
        Invalidate();
	}
    GLFramebuffer::~GLFramebuffer()
	{
        glDeleteFramebuffers(1, &ID);
        glDeleteTextures(1, &ColorAttachment);
        glDeleteTextures(1, &DepthAttachment);
	}

    void GLFramebuffer::Invalidate()
    {
        if (ID)
		{
			glDeleteFramebuffers(1, &ID);
			glDeleteTextures(1, &ColorAttachment);
			glDeleteTextures(1, &DepthAttachment);
		}

		glCreateFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Spec.Width, Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Spec.Width, Spec.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);

		if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)) LOG_ERR("Framebuffer is incomplete!\n");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFramebuffer::Bind()
	{
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
        glViewport(0, 0, Spec.Width, Spec.Height);
	}
    void GLFramebuffer::Unbind()
	{
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void GLFramebuffer::Resize(unsigned int width, unsigned int height)
	{
        if (width == 0 || height == 0 || width > C_MaxFrameBufferSize || height > C_MaxFrameBufferSize)
		{
			LOG_WRN("Attempted to rezize framebuffer to %d, %d\n", width, height);
			return;
		}
		Spec.Width = width;
		Spec.Height = height;
		
		Invalidate();
	}

}