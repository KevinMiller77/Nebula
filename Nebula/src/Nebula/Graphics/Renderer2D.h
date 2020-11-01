#pragma once

#include <Nebula.h>
#include <vector>
#include <array>
#include <Graphics/OrthographicCamera.h>
#include <Core/VFS.h>


namespace Nebula
{
    class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera* camera);
		static void BeginScene(Camera* camera, const Mat4f transform);
		static void EndScene();
		static void Flush();

		//TEMP:
		static void SetShader(std::string path);

		// Primitives
		static void DrawQuad(const Vec2f& position, const Vec2f& size, const Vec4f& color);
		static void DrawQuad(const Vec3f& position, const Vec2f& size, const Vec4f& color);
		static void DrawQuad(const Vec2f& position, const Vec2f& size, const Texture2D* texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));
		static void DrawQuad(const Vec3f& position, const Vec2f& size, const Texture2D* texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));

		static void DrawQuad(const Mat4f& transform, const Vec4f& color);
		static void DrawQuad(const Mat4f& transform, const Texture2D*& texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));

		static void DrawRotatedQuad(const Vec2f& position, const Vec2f& size, float rotation, const Vec4f& color);
		static void DrawRotatedQuad(const Vec3f& position, const Vec2f& size, float rotation, const Vec4f& color);
		static void DrawRotatedQuad(const Vec2f& position, const Vec2f& size, float rotation, const Texture2D* texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));
		static void DrawRotatedQuad(const Vec3f& position, const Vec2f& size, float rotation, const Texture2D* texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();
	};
}