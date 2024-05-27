#pragma once

#include <Graphics/Texture.h>
#include <Core/NebulaCommon.h>

#include <Metal/MTLPixelFormat.hpp>

namespace MTL {
    class Texture;
}

namespace Nebula{
    class MetalTexture2D : public Texture2D
	{
	public:
		MetalTexture2D(uint32_t width, uint32_t height, bool child = false);
		MetalTexture2D(const std::string& path, bool child = false);

		// Reserved for MetalTileMap, textureCoords MUST already be normalize
		MetalTexture2D(MetalTexture2D* parent, int width, int height, Mat42f textureCoords, bool child = true) { LOG_ERR("Metal Tile Map not implemented yet!"); abort(); };

		virtual ~MetalTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width;  }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual std::string GetPath() override { return m_Path; };
		virtual void* GetRendererID() const override { return m_Texture; }
		virtual Mat42f GetTexCoords() const override { return m_TextureCoords; };
		virtual uint32_t GetIntFormat() { return m_IntFormat; } 
		virtual uint32_t GetDataFormat()  { return m_IntFormat; }
		virtual Ref<Texture2D> GetSubTexture(int xPosPixels, int yPosPixels, int xSizePixels = 16, int yTileSize = 16) override;
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_Texture == ((MetalTexture2D&)other).m_Texture;
		}
	private:
		std::string m_Path;

		uint32_t m_Width, m_Height;
		MTL::Texture* m_Texture;
		std::uintptr_t m_IntFormat;
		Mat42f m_TextureCoords = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		bool m_ChildTexture = false;

		// friend class MetalTileMap;
	};
}