#pragma once

#include <Graphics/Texture.h>

namespace Nebula {
    class GLTextureCube : public TextureCube
	{
	public:
		GLTextureCube(TextureFormat format, uint32_t width, uint32_t height);
		GLTextureCube(const std::string& path);
		virtual ~GLTextureCube();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual TextureFormat GetFormat() const { return m_Format; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual Mat42f GetTexCoords() const override { return m_TextureCoords; }
        
        // TODO: Implement SetData
		virtual void SetData(void* data, uint32_t size) override { LOG_ERR("Cannot set data of a texture cube!!\n"); assert((false)); };
		// This function currently returns the expected number of mips based on image size,
		// not present mips in data
		virtual uint32_t GetMipLevelCount() const override;

		virtual std::string GetPath() override { return m_FilePath; }

		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((GLTextureCube&)other).m_RendererID;
		}
	private:
		uint32_t m_RendererID;
		TextureFormat m_Format;
		uint32_t m_Width, m_Height;
		Mat42f m_TextureCoords = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		unsigned char* m_ImageData;

		std::string m_FilePath;
	};
}