#pragma once

#include <Graphics/Texture.h>
#include <Core/NebulaCommon.h>

typedef uint32 GLenum;

namespace Nebula
{
    class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(uint32_t width, uint32_t height);
		GLTexture2D(const std::string& path);
		virtual ~GLTexture2D();

		virtual uint32_t GetWidth() const override { return Width;  }
		virtual uint32_t GetHeight() const override { return Height; }
		virtual std::string GetPath() override { return Path; };
		virtual uint32_t GetRendererID() const override { return ID; }
		
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
	};
}