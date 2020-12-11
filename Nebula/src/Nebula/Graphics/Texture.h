#pragma once

#include <string>
#include <Core/Ref.h>
#include <Math/Math.h>

namespace Nebula
{
    class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::string GetPath() = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual Mat42f GetTexCoords() const = 0;

		//TODO: Need to have my own enum for texture byte types
		virtual uint32_t GetIntFormat() = 0; 
		virtual uint32_t GetDataFormat() = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		virtual Ref<Texture2D> GetSubTexture(int xPosPixels, int yPosPixels, int xSizePixels, int yTileSize) = 0;

		inline bool IsValid() { return m_Valid; } 

	protected:
		bool m_Valid;
	};

	class TileMap
	{
	public:
		TileMap(std::string path, int xTilePixels, int yTilePixels);
		TileMap(Ref<Texture2D> tex, int xTilePixels, int yTilePixels);

		Ref<Texture2D> GetTexture() { return m_ParentTexture; }
		inline Vec2i GetResolution() { return { m_XTilePixels, m_YTilePixels }; }

		Ref<Texture2D> GetTileAt(int x, int y, int xSize = 1, int ySize = 1);

	private:
		Ref<Texture2D> m_ParentTexture;
		int m_XTilePixels, m_YTilePixels;
	};

}