#include "MetalTexture2D.h"
#include <Nebula_pch.h>

#include <Metal/Metal.hpp>
#include <stb_image.h>

#include <Utils/Logging.h>
#include <Graphics/RendererAPI.h>
#include <Platform/Graphics/Metal/MetalRendererAPI.h>
#include <Platform/Graphics/Metal/MetalContext.h>


namespace Nebula{
    MetalTexture2D::MetalTexture2D(uint32_t width, uint32_t height, bool child)
		: m_Width(width), m_Height(height), m_ChildTexture(child)
	{
		if (MetalRendererAPI::s_Context == nullptr) {
			LOG_ERR("[Metal][Texture2D] MetalRendererAPI not initialized!\n");
			return;
		}

		NEB_PROFILE_FUNCTION();
		m_Path = std::string();
		m_IntFormat = MTL::PixelFormatRGBA8Unorm;

        MTL::TextureDescriptor* desc = MTL::TextureDescriptor::texture2DDescriptor(
            (MTL::PixelFormat)m_IntFormat, 
            m_Width, 
            m_Height, 
            false
        );

        if (!desc) {
            LOG_ERR("[Metal][Texture2D] Failed to allocate texture descriptor!\n");
            return;
        }

        desc->setStorageMode(MTL::StorageModeShared);
        desc->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);

        MTL::Texture* tex = MetalRendererAPI::GetDevice()->newTexture(desc);
        if (!tex) {
            LOG_ERR("[Metal][Texture2D] Failed to allocate texture!\n");
            return;
        }

		m_Texture = tex;
	}

	MetalTexture2D::MetalTexture2D(const std::string& path, bool child)
		: m_Path(path), m_ChildTexture(child)
	{
		if (MetalRendererAPI::s_Context == nullptr) {
			LOG_ERR("[Metal][Texture2D] MetalRendererAPI not initialized!\n");
			return;
		}

		if (MetalRendererAPI::GetDevice() == nullptr) {
			LOG_ERR("[Metal][Texture2D] MetalRendererAPI device not initialized!\n");
			return;
		}

		m_Valid = true;
		NEB_PROFILE_FUNCTION();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
        if(!data)
		{	
			m_Valid = false;
			LOG_ERR("[Metal][Texture2D] Failed to load image for texture!\n----> Path was: %s\n", path.c_str());
		}
		
		// Add the 4th channel
		if (channels == 3) {
			LOG_WRN("[Metal][Texture2D] Image loaded with 3 channels, adding alpha channel! Path: %s\n", path.c_str());
			stbi_uc* dataWithAlpha = (stbi_uc*)malloc(width * height * 4);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					for (int c = 0; c < 3; c++) {
						dataWithAlpha[(y * width + x) * 4 + c] = data[(y * width + x) * 3 + c];
					}
					dataWithAlpha[(y * width + x) * 4 + 3] = 255;
				}
			}
			stbi_image_free(data);
			data = dataWithAlpha;
			channels = 4;
		}

        if (channels != 4)
		{
            LOG_ERR("[Metal][Texture2D] Only four channel texture images are supported on Metal!\n");
            abort();
        }

		m_Width = width;
		m_Height = height;
        m_IntFormat = MTL::PixelFormatRGBA8Unorm;

		MTL::TextureDescriptor* desc = MTL::TextureDescriptor::texture2DDescriptor(
			(MTL::PixelFormat)m_IntFormat, 
            m_Width, 
            m_Height, 
            false
        );

        if (!desc) {
            LOG_ERR("[Metal][Texture2D] Failed to allocate texture descriptor!\n");
            return;
        }

        desc->setStorageMode(MTL::StorageModeShared);
        desc->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);

        MTL::Texture* tex = MetalRendererAPI::GetDevice()->newTexture(desc);
        if (!tex) {
            LOG_ERR("[Metal][Texture2D] Failed to allocate texture!\n");
            return;
        }

		m_Texture = tex;
		SetData(data, m_Width * m_Height * 4);

		stbi_image_free(data);
	}

	// MetalTexture2D::MetalTexture2D(MetalTexture2D* parent, int width, int height, Mat42f textureCoords, bool child)
	// 	: ChildTexture(child)
	// {
	// 	if (!parent)
	// 	{
	// 		LOG_ERR("Could not make child texture! Parent was null\n");
	// 	}

	// 	// Most information is just copied from the parent, we dont have to
	// 	// load the texture twice
	// 	Path = parent->GetPath(); ID = parent->GetRendererID();
	// 	IntFormat = (Metalenum)parent->GetIntFormat(); DataFormat = (Metalenum)parent->GetDataFormat();

	// 	Width = width; 
	// 	Height = height;
	// 	TextureCoords = textureCoords;
	// }

	
	MetalTexture2D::~MetalTexture2D()
	{
		if(!m_ChildTexture)
		{
			LOG_WRN("[Metal][Texture2D] TODO: Implement texture releasing for METAL!\n");
		}
	}

	void MetalTexture2D::SetData(void* data, uint32_t size)
	{
		NEB_PROFILE_FUNCTION();
		
		if (!m_Texture) {
			LOG_ERR("[Metal][Texture2D] Attempted to set a Metal texture data but texture is not initialzed!\n");
			return;
		}

		size_t bytesPerPixel = 4;
		if (size != m_Width * m_Height * bytesPerPixel) {
			LOG_ERR("[Metal][Texture2D] Data must be entire texture!\n");
			abort();
		}

		size_t bytesPerRow = m_Width * bytesPerPixel;
		m_Texture->replaceRegion(
			MTL::Region::Make2D(0, 0, m_Width, m_Height), 
			0, // Level is always 0 for now
			data, 
			bytesPerRow
		);

		// glTextureSubImage2D(ID, 0, 0, 0, Width, Height, DataFormat, Metal_UNSIGNED_BYTE, data);
	}

	void MetalTexture2D::Bind(uint32_t slot) const { 
		if (!m_Texture) {
			LOG_ERR("[Metal][Texture2D] Attempted to bind a Metal texture but texture is not initialzed!\n");
			return;
		}

		MetalRendererAPI::BindShaderFragmentTexture(m_Texture, slot);
	}

	//	Pixel counts in
	Ref<Texture2D> MetalTexture2D::GetSubTexture(int xPosPixels, int yPosPixels, int xSizePixels, int ySizePixels)
	{
		float w = (float)m_Width;
		float h = (float)m_Height;

		Vec2f botL = { (float)(xPosPixels) 				 / w ,  (yPosPixels) / h };  // 0, 1
		Vec2f botR = { (float)(xPosPixels + xSizePixels) / w , (yPosPixels)	/ h	};  //  1, 1
		Vec2f topL = { (float)(xPosPixels) 				 / w , (yPosPixels + ySizePixels) / h	}; // 0, 0
		Vec2f topR = { (float)(xPosPixels + xSizePixels) / w , (yPosPixels + ySizePixels) / h 	};  // 1 0

		Mat42f out = { botL, botR, topR, topL};

		return CreateRef<MetalTexture2D>(this, xSizePixels, ySizePixels, out);
	}
}