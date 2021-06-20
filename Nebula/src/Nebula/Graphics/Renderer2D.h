#pragma once

#include <Nebula.h>
#include <vector>
#include <array>
#include <Graphics/OrthographicCamera.h>
#include <Core/VFS.h>
#include <Core/Ref.h>
#include "EditorCamera.h"


namespace Nebula{
	struct QuadVertex
	{
		Vec3f Translation;
        Vec3f Rotation;
        Vec3f Scale;
        Vec4f QuadIndexPos;
		Vec4f Color;
		Vec2f TexCoord;
		float TexIndex;
		float TilingFactor;
        int   EntityID;
        // Mat4f ModelMat;
	};

	struct LineVertex
	{
		Vec3f Position;
		Vec4f Color;
	};

	struct Renderer2DStatistics
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t LineCount = 0;

		uint32_t GetTotalVertexCount() { return QuadCount * 4 + LineCount * 2; }
		uint32_t GetTotalIndexCount() { return QuadCount * 6 + LineCount * 2; }
	};
	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 100000;
		static const uint32_t MaxQuadVertices = MaxQuads * 4;
		static const uint32_t MaxQuadIndices = MaxQuads * 6;

		static const uint32_t MaxLines = 10000;
		static const uint32_t MaxLineVertices = MaxLines * 2;
		static const uint32_t MaxLineIndices = MaxLines * 6;

		// TODO: Check actual number on machine
		// Should I though? It will always be >=32 
		static const uint32_t MaxTextureSlots = 32; 

		Mat4f viewProj;
		
		Ref<VertexArray> 	QuadVertexArray;
		Ref<VertexBuffer> 	QuadVertexBuffer;
		Ref<IndexBuffer>	QuadIndexBuffer;

		Ref<Shader> 		TextureShader;
		Ref<Texture2D> 		WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		Vec4f QuadVertexPositions[4];

		Ref<VertexArray> 	LineVertexArray;
		Ref<VertexBuffer> 	LineVertexBuffer;
		Ref<IndexBuffer> 	LineIndexBuffer;

		Ref<Shader> LineShader;

		uint32_t LineIndexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

        bool DepthTest = true;

		Renderer2DStatistics Stats;
	};
    class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(Camera& camera, const Mat4f transform, bool depthTest);
        static void BeginScene(EditorCamera& camera, bool depthTest);
		static void BeginScene(Mat4f viewProj, bool depthTest);
		static void EndScene();
		static void Flush();

		//TEMP:
		static void SetShader(std::string path);
		static void ReloadShaders();

		// Primitives
		static void DrawQuad(const Mat4f& modelMat, const int entityID, const Vec2f& position, const Vec3f& scale, Vec3f& rotation, const Vec4f& color);
		static void DrawQuad(const Mat4f& modelMat, const int entityID, const Vec3f& position, const Vec3f& scale, Vec3f& rotation, const Vec4f& color);
		static void DrawQuad(const Mat4f& modelMat, const int entityID, const Vec2f& position, const Vec3f& scale, Vec3f& rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));
		static void DrawQuad(const Mat4f& modelMat, const int entityID, const Vec3f& position, const Vec3f& scale, Vec3f& rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));

        /* Depracted with the newer shader
		static void DrawQuad(const Mat4f& transform, const Vec4f& color);
		static void DrawQuad(const Mat4f& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));
		static void DrawRotatedQuad(const Vec2f& position, const Vec2f& size, float rotation, const Vec4f& color);
		static void DrawRotatedQuad(const Vec3f& position, const Vec2f& size, float rotation, const Vec4f& color);
		static void DrawRotatedQuad(const Vec2f& position, const Vec2f& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));
		static void DrawRotatedQuad(const Vec3f& position, const Vec2f& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const Vec4f& tintColor = Vec4f(1.0f));
        */

		static void DrawLine(const Vec3f& p0, const Vec3f& p1, const Vec4f& color = Vec4f(1.0f));

		// Stats
		static void ResetStats();
		static Renderer2DStatistics GetStats();
	private:
		static void FlushAndReset();
		static void FlushAndResetLines();
	};
}