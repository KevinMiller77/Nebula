#include "Pipeline.h"
#include "RendererAPI.h"
#include <Platform/Graphics/GL/GLPipeline.h>

namespace Nebula{
    Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLPipeline>(spec);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
}