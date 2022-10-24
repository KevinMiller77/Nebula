#include "GLTexture.h"

#include <array>

#include <glad/glad.h>
#include <Graphics/Renderer.h>
#include <stb_image/stb_image.h>

namespace Nebula {

    static GLenum NebToOpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGB:     return GL_RGB;
			case TextureFormat::RGBA:    return GL_RGBA;
			case TextureFormat::Float16: return GL_RGBA16F;
			default: {}
		}
		LOG_ERR("Unknown texture format!\n");
		return 0;
	}

    GLTextureCube::GLTextureCube(TextureFormat format, uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		m_Format = format;

		uint32_t levels = Texture::CalculateMipMapCount(width, height);
		GLTextureCube* instance = this;
		Renderer::Submit([instance, levels]() mutable
		{
			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &instance->m_RendererID);
			glTextureStorage2D(instance->m_RendererID, levels, NebToOpenGLTextureFormat(instance->m_Format), instance->m_Width, instance->m_Height);
			glTextureParameteri(instance->m_RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
			glTextureParameteri(instance->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// glTextureParameterf(m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, 16);
		});
	}

	// TODO: Revisit this, as currently env maps are being loaded as equirectangular 2D images
	//       so this is an old path
	GLTextureCube::GLTextureCube(const std::string& path)
		: m_FilePath(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGB;

		uint32_t faceWidth = m_Width / 4;
		uint32_t faceHeight = m_Height / 3;
        if (faceWidth != faceHeight) {
            LOG_ERR("Non-square faces!\n");
        }

		std::array<uint8_t*, 6> faces;
		for (size_t i = 0; i < faces.size(); i++)
			faces[i] = new uint8_t[faceWidth * faceHeight * 3]; // 3 BPP

		int faceIndex = 0;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + i * faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + i * faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		GLTextureCube* instance = this;
		Renderer::Submit([instance, faceWidth, faceHeight, faces]() mutable
		{
			glGenTextures(1, &instance->m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, instance->m_RendererID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			// glTextureParameterf(instance->m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

			auto format = NebToOpenGLTextureFormat(instance->m_Format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[3]);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_2D, 0);

			for (size_t i = 0; i < faces.size(); i++)
				delete[] faces[i];

			stbi_image_free(instance->m_ImageData);
		});
	}

	GLTextureCube::~GLTextureCube()
	{
		GLuint rendererID = m_RendererID;
		Renderer::Submit([rendererID]() {
			glDeleteTextures(1, &rendererID);
		});
	}

	void GLTextureCube::Bind(uint32_t slot) const
	{
		const GLTextureCube* instance = this;
		Renderer::Submit([instance, slot]() {
			glBindTextureUnit(slot, instance->m_RendererID);
		});
	}

	uint32_t GLTextureCube::GetMipLevelCount() const
	{
		return Texture::CalculateMipMapCount(m_Width, m_Height);
	}
}