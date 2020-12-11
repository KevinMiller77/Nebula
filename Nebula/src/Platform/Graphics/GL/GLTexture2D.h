#pragma once

#include <Graphics/Texture.h>
#include <Core/NebulaCommon.h>

typedef uint32 GLenum;

namespace Nebula
{
    class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(uint32_t width, uint32_t height, bool child = false);
		GLTexture2D(const std::string& path, bool child = false);

		// Reserved for GLTileMap, textureCoords MUST already be normalize
		GLTexture2D(GLTexture2D* parent, int width, int height, Mat42f textureCoords, bool child = true);

		virtual ~GLTexture2D();

		virtual uint32_t GetWidth() const override { return Width;  }
		virtual uint32_t GetHeight() const override { return Height; }
		virtual std::string GetPath() override { return Path; };
		virtual uint32_t GetRendererID() const override { return ID; }
		virtual Mat42f GetTexCoords() const override { return TextureCoords; };
		virtual uint32_t GetIntFormat() override { return IntFormat; } 
		virtual uint32_t GetDataFormat() override { return DataFormat; }
		virtual Ref<Texture2D> GetSubTexture(int xPosPixels, int yPosPixels, int xSizePixels = 16, int yTileSize = 16) override;
		
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return ID == ((GLTexture2D&)other).ID;
		}
	private:
		std::string Path;
		uint32_t Width, Height;
		uint32_t ID;
		GLenum IntFormat, DataFormat;
		Mat42f TextureCoords = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		bool ChildTexture = false;

		friend class GLTileMap;
	};
}