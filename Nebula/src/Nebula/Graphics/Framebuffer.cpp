#include "Framebuffer.h"
#include <Platform/Graphics/GL/GLFramebuffer.h>

#include <Nebula.h>

namespace Nebula
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch(RendererAPI::GetAPI())
        {
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLFramebuffer>(spec);
		}

        LOG_ERR("No rendering API selected!\n");
        return nullptr;
    }
}