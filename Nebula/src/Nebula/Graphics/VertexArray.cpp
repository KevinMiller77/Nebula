#include "VertexArray.h"

#include "RendererConfig.h"
#include <Platform/Graphics/GL/GLVertexArray.h>

namespace Nebula{
    Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLVertexArray>();
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
}