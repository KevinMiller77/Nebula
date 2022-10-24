#include "Buffer.h"

#include "RendererConfig.h"
#include <Platform/Graphics/GL/GLBuffers.h>

#include <Nebula_pch.h>

namespace Nebula{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLVertexBuffer>(size);
			default: { LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr; }
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

	 Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLVertexBuffer>(vertices, size);
			default: { LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr; }
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLIndexBuffer>(indices, size);
			default: { LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr; }
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
}