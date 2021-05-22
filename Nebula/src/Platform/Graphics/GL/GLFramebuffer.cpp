#include "GLFramebuffer.h"

#include <Nebula_pch.h>
#include <Utils/Logging.h>
#include <glad/glad.h>

#include <ext/imgui/examples/imgui_impl_opengl3.h>
namespace Nebula
{
    static const uint32 C_MaxFrameBufferSize = 8192;

    static GLenum TextureTarget(bool multisampled) {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    namespace Utils {
        static bool IsDepthFormat(FramebufferTextureSpecification format) {
            switch (format.TextureFormat) {
                case(FramebufferTextureFormat::DEPTH24STENCIL8): {
                    return true;
                } 
                default:
                    return false;
            }
        }

        static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void BindTexture(bool multisampled, uint32_t id) {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(uint32_t colorAttachment, int samples, GLenum format, uint32_t width, uint32_t height, int index) {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampled, format, width, height, GL_FALSE);
            }
            else
            {
                // TODO: Extract data type from format
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), colorAttachment, 0);
        }

        static void AttachDepthTexture(uint32 depthAttachment, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampled, format, width, height, GL_FALSE);
            }
            else
            {
                // TODO: Extract data type from format
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), depthAttachment, 0);
        }
    }

    GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec)
        :   Spec(spec)
    {
		NEB_PROFILE_FUNCTION();

        for (auto format : Spec.Attachments.TextureAttachments) {
            if (!Utils::IsDepthFormat(format)) {
                m_ColorAttachmentSpecs.emplace_back(format);
            } else {
                m_DepthAttachmentSpec = format;
            }
        }

		Resize(spec.Width, spec.Height);

	}
    GLFramebuffer::~GLFramebuffer()
	{
		NEB_PROFILE_FUNCTION();
        glDeleteFramebuffers(1, &ID);
        glDeleteTextures(1, &m_DepthAttachment);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
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

    void GLFramebuffer::Resize(uint32 width, uint32 height)
	{
		NEB_PROFILE_FUNCTION();

		Spec.Width = width;
		Spec.Height = height;

        Invalidate();		
	}

    void GLFramebuffer::Invalidate() 
    {
        if (ID)
		{
			glDeleteFramebuffers(1, &ID);
			glDeleteTextures(1, &m_DepthAttachment);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
		}

		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

		bool multisample = Spec.Samples > 1;
        if (m_ColorAttachmentSpecs.size()) {

            m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
            Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
                Utils::BindTexture(multisample, m_ColorAttachments[i]);
                switch(m_ColorAttachmentSpecs[i].TextureFormat) {
                    case(FramebufferTextureFormat::RGBA8): {
                        Utils::AttachColorTexture(m_ColorAttachments[i], Spec.Samples, GL_RGBA8, Spec.Width, Spec.Height, i);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None) {
            Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
            Utils::BindTexture(multisample, m_DepthAttachment);
            switch(m_DepthAttachmentSpec.TextureFormat) {
                case(FramebufferTextureFormat::DEPTH24STENCIL8): {
                    Utils::AttachDepthTexture(m_DepthAttachment, Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, Spec.Width, Spec.Height);
                }
            }
        }

        if (m_ColorAttachments.size() > 1) {
            assert(m_ColorAttachments.size() <= 4);
            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty()) {
            glDrawBuffer(GL_NONE);
        }

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}