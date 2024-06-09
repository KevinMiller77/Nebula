#include "RenderPass.h"
#include "RendererAPI.h"
#include <Platform/Graphics/GL/GLRenderPass.h>
#include <Platform/Graphics/Metal/MetalRenderPass.h>

namespace Nebula{
    Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLRenderPass>(spec);
			case RendererAPI::API::Metal:   return CreateRef<MetalRenderPass>(spec);
			default: { LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr; }
		}

		LOG_ERR("[RenderPass] Unknown RendererAPI!\n");
		return nullptr;
	}
}