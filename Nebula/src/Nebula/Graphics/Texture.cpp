#include "Texture.h"

#include <Graphics/RendererAPI.h>
#include <Platform/Graphics/GL/GLTexture2D.h>

namespace Nebula
{
    Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLTexture2D(width, height);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

	Texture2D* Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return new GLTexture2D(path);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
}