#include "Texture.h"

#include <Graphics/RendererAPI.h>
#include <Platform/Graphics/GL/GLTexture2D.h>
#include <Platform/Graphics/GL/GLTexture.h>

namespace Nebula{

    uint32_t Texture::GetBPP(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGB:    return 3;
			case TextureFormat::RGBA:   return 4;
		}
		return 0;
	}

	uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height)
	{
		uint32_t levels = 1;
		while ((width | height) >> levels)
			levels++;

		return levels;
	}
    
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLTexture2D>(width, height);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    LOG_ERR("RendererAPI::None is currently not supported!\n"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<GLTexture2D>(path);
		}

		LOG_ERR("Unknown RendererAPI!\n");
		return nullptr;
	}
	

	// Tile map ... completely abstracted (as it should be)
	TileMap::TileMap(std::string path, int xTilePixels, int yTilePixels)
		: 	m_XTilePixels(xTilePixels), m_YTilePixels(yTilePixels)
	{
		m_ParentTexture = Texture2D::Create(path);
	}
	
	TileMap::TileMap(Ref<Texture2D> tex, int xTilePixels, int yTilePixels)
		: m_ParentTexture(tex), m_XTilePixels(xTilePixels), m_YTilePixels(yTilePixels) {}

	//	Index in
	Ref<Texture2D> TileMap::GetTileAt(int x, int y, int xSize, int ySize) 
	{
		// Convert to pixel values and get subtex
		return m_ParentTexture->GetSubTexture(x * m_XTilePixels, y * m_YTilePixels, xSize * m_XTilePixels, ySize * m_YTilePixels);
	}

    Ref<TextureCube> TextureCube::Create(TextureFormat format, uint32_t width, uint32_t height)
	{
		switch (RendererAPI::GetAPI())

		{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<GLTextureCube>(format, width, height);
			//case RendererAPIType::Vulkan: return Ref<VulkanTextureCube>::Create(format, width, height);
		}
		LOG_ERR("Unknown RendererAPI\n");
		return nullptr;
	}

	Ref<TextureCube> TextureCube::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<GLTextureCube>(path);
			//case RendererAPIType::Vulkan: return Ref<VulkanTextureCube>::Create(path);
		}
		LOG_ERR("Unknown RendererAPI\n");
		return nullptr;
	}


}