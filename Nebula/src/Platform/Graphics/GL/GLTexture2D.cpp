#include "GLTexture2D.h"

#include <stb_image/stb_image.h>

#include <Nebula_pch.h>
#include <glad/glad.h>
#include <Utils/Logging.h>
namespace Nebula
{
    GLTexture2D::GLTexture2D(uint32_t width, uint32_t height)
		: Width(width), Height(height)
	{
		NEB_PROFILE_FUNCTION();
		Path = std::string();
		IntFormat = GL_RGBA8;
		DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &ID);
		glTextureStorage2D(ID, 1, IntFormat, Width, Height);

		glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	GLTexture2D::GLTexture2D(const std::string& path)
		: Path(path)
	{
		valid = true;
		NEB_PROFILE_FUNCTION();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
        if(!data)
		{	
			valid = false;
			LOG_ERR("Failed to load image for texture!\n----> Path was: %s\n", path.c_str());
		}
		Width = width;
		Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		IntFormat = internalFormat;
		DataFormat = dataFormat;

		if(!(internalFormat & dataFormat)) 
		{
			valid = false;
			LOG_ERR("Texture image format not supported!\n");
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &ID);
		glTextureStorage2D(ID, 1, internalFormat, Width, Height);

		glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(ID, 0, 0, 0, Width, Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &ID);
	}

	void GLTexture2D::SetData(void* data, uint32_t size)
	{
		NEB_PROFILE_FUNCTION();
		uint32_t bpp = DataFormat == GL_RGBA ? 4 : 3;
		if(!(size == Width * Height * bpp)) LOG_ERR("Data must be entire texture!\n");
		glTextureSubImage2D(ID, 0, 0, 0, Width, Height, DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void GLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, ID);
	}
}