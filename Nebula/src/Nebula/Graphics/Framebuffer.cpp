#include "Framebuffer.h"
#include <Platform/Graphics/GL/GLFramebuffer.h>
#include <Platform/Graphics/Metal/MetalFramebuffer.h>

#include <Nebula.h>

namespace Nebula{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch(RendererAPI::GetAPI())
        {
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLFramebuffer>(spec);
            case RendererAPI::API::Metal:   return CreateRef<MetalFramebuffer>(spec);
            default: { LOG_ERR("RendererAPI::None is currently not supported!"); return nullptr; }
		}

        LOG_ERR("No rendering API selected!\n");
        return nullptr;
    }
}