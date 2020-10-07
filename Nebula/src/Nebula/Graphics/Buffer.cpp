#include "Buffer.h"

#include "RendererConfig.h"
#include <Platform/Graphics/GL/GLBuffers.h>

#include <Nebula_pch.h>

namespace Nebula
{
    VertexBuffer* VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLVertexBuffer(size);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLVertexBuffer(vertices, size);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLIndexBuffer(indices, size);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
}