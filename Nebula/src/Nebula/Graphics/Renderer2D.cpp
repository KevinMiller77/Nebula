#include "Renderer2D.h"
#include "BuiltinShaders.h"
namespace Nebula{
	static Renderer2DData s_2DData;

	void Renderer2D::SetShader(std::string path)
	{
		int32_t samplers[s_2DData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_2DData.MaxTextureSlots; i++)
			samplers[i] = i;

		//TODO: Material system that handles shaders
		s_2DData.TextureShader = Shader::Create(VFS::AbsolutePath(path));
		s_2DData.TextureShader->Bind();
		s_2DData.TextureShader->SetUniform("u_Textures", samplers, s_2DData.MaxTextureSlots);
	}

	void Renderer2D::ReloadShaders()
	{
		// TexQuad
		s_2DData.TextureShader->Reload(true);
		s_2DData.TextureShader->Bind();

		// Tex samplers
		int32_t samplers[s_2DData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_2DData.MaxTextureSlots; i++) {
			samplers[i] = i;
		}
		s_2DData.TextureShader->SetUniform("u_Textures", samplers, s_2DData.MaxTextureSlots);
		
		// Line
		s_2DData.LineShader->Reload(true);
	}

	void Renderer2D::Init()
	{
		// Init Quad Props
		s_2DData.QuadVertexArray = VertexArray::Create();

		s_2DData.QuadVertexBuffer = VertexBuffer::Create(s_2DData.MaxQuadVertices * sizeof(QuadVertex));
		s_2DData.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Translation" },
            { ShaderDataType::Float3, "a_Rotation" },
            { ShaderDataType::Float3, "a_Scale" },
            { ShaderDataType::Float4, "a_QuadIndexPos" },

			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
            { ShaderDataType::Int, "a_EntityID" },
            // { ShaderDataType::Mat4,   "a_ModelMat"}
			});
		s_2DData.QuadVertexArray->AddVertexBuffer(s_2DData.QuadVertexBuffer);

		s_2DData.QuadVertexBufferBase = new QuadVertex[s_2DData.MaxQuadVertices];

		uint32_t* quadIndices = new uint32_t[s_2DData.MaxQuadIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_2DData.MaxQuadIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_2DData.QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_2DData.MaxQuadIndices);
		s_2DData.QuadVertexArray->SetIndexBuffer(s_2DData.QuadIndexBuffer);
		delete[] quadIndices;

		s_2DData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_2DData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_2DData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_2DData.MaxTextureSlots; i++)
			samplers[i] = i;

		s_2DData.TextureShader = Renderer::GetShaderLibrary()->Get("TexQuad");
		assert(s_2DData.TextureShader);

		s_2DData.TextureShader->Bind();
		s_2DData.TextureShader->SetUniform("u_Textures", samplers, s_2DData.MaxTextureSlots);

		// Set first texture slot to 0
		s_2DData.TextureSlots[0] = s_2DData.WhiteTexture;

		s_2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Init Line Props

		s_2DData.LineShader = Renderer::GetShaderLibrary()->Get("Line");
		s_2DData.LineVertexArray = VertexArray::Create();
			
		s_2DData.LineVertexBuffer = VertexBuffer::Create(s_2DData.MaxLineVertices * sizeof(LineVertex));
		s_2DData.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		});
		s_2DData.LineVertexArray->AddVertexBuffer(s_2DData.LineVertexBuffer);

		s_2DData.LineVertexBufferBase = new LineVertex[s_2DData.MaxLineVertices];

		uint32_t* lineIndicies = new uint32_t[s_2DData.MaxLineIndices];
		for (uint32_t i = 0; i < s_2DData.MaxLineIndices; i++)
		{
			lineIndicies[i] = i;
		}
		s_2DData.LineIndexBuffer = IndexBuffer::Create(lineIndicies, s_2DData.MaxLineIndices);
		s_2DData.LineVertexArray->SetIndexBuffer(s_2DData.LineIndexBuffer);
		delete[] lineIndicies;
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_2DData.QuadVertexBufferBase;
		delete[] s_2DData.LineVertexBufferBase;
	}

	void Renderer2D::BeginScene(Camera& camera, Mat4f transform, bool depthTest)
	{
		ResetStats();

        s_2DData.DepthTest = depthTest;
		s_2DData.viewProj = camera.GetProjection() * transform.invertMatrix();

		if (s_2DData.TextureShader)
		{
			s_2DData.TextureShader.get()->Bind();
			s_2DData.TextureShader->SetUniformBuffer("Camera", s_2DData.viewProj.elements, sizeof(Vec4f) * 4);
		}

		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.LineIndexCount = 0;
		s_2DData.LineVertexBufferPtr = s_2DData.LineVertexBufferBase;

		s_2DData.TextureSlotIndex = 1;
	}

    void Renderer2D::BeginScene(EditorCamera& camera, bool depthTest)
	{
		ResetStats();

        s_2DData.DepthTest = depthTest;
        
        Renderer2D::DrawLine({-50000, 0, 0}, {50000, 0, 0}, {1.0, 1.0, 1.0, 1.0});

		s_2DData.viewProj = camera.GetViewProjection();

		if (s_2DData.TextureShader)
		{
			s_2DData.TextureShader.get()->Bind();
			s_2DData.TextureShader->SetUniformBuffer("Camera", s_2DData.viewProj.elements, sizeof(Vec4f) * 4);
		}

		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.LineIndexCount = 0;
		s_2DData.LineVertexBufferPtr = s_2DData.LineVertexBufferBase;

		s_2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(Mat4f viewProj, bool depthTest)
	{
		ResetStats();

        s_2DData.DepthTest = depthTest;
        
		s_2DData.viewProj = viewProj;

		if (s_2DData.TextureShader)
		{
			s_2DData.TextureShader->Bind();
			s_2DData.TextureShader->SetUniformBuffer("Camera", s_2DData.viewProj.elements, sizeof(Vec4f) * 4);
		}

		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.LineIndexCount = 0;
		s_2DData.LineVertexBufferPtr = s_2DData.LineVertexBufferBase;

		s_2DData.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		// Quads
		uint32_t data = (uint8_t*)s_2DData.QuadVertexBufferPtr - (uint8_t*)s_2DData.QuadVertexBufferBase;
		if (data)
		{
			s_2DData.QuadVertexBuffer->SetData(s_2DData.QuadVertexBufferBase, data);
			
			s_2DData.TextureShader->Bind();
			s_2DData.TextureShader->SetUniformBuffer("Camera", s_2DData.viewProj.elements, sizeof(Vec4f) * 4);

			// Bind textures
			for (uint32_t i = 0; i < s_2DData.TextureSlotIndex; i++)
				s_2DData.TextureSlots[i]->Bind(i);
			
			s_2DData.QuadVertexArray->Bind();
			s_2DData.QuadIndexBuffer->Bind();
            
			RendererConfig::DrawIndexed(s_2DData.QuadIndexCount, PrimativeType::TRIANGLES, s_2DData.DepthTest);
			s_2DData.Stats.DrawCalls++;
		}
		//Lines
		data = (uint8_t*)s_2DData.LineVertexBufferPtr - (uint8_t*)s_2DData.LineVertexBufferBase;
		if (data)
		{
			s_2DData.LineVertexBuffer->SetData(s_2DData.LineVertexBufferBase, data);

			s_2DData.LineShader->Bind();
			s_2DData.TextureShader->SetUniformBuffer("Camera", s_2DData.viewProj.elements, sizeof(Vec4f) * 4);

			s_2DData.LineVertexArray->Bind();
			s_2DData.LineIndexBuffer->Bind();
			
			RendererConfig::DrawIndexed(s_2DData.LineIndexCount, PrimativeType::LINES, s_2DData.DepthTest);
			s_2DData.Stats.DrawCalls++;
		}
	}

	void Renderer2D::Flush()
	{}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.LineIndexCount = 0;
		s_2DData.LineVertexBufferPtr = s_2DData.LineVertexBufferBase;

		s_2DData.TextureSlotIndex = 1;
	}

	
	void Renderer2D::FlushAndResetLines()
	{

	}


	void Renderer2D::DrawQuad(const Mat4f& modelMat, const int entityID,  const Vec2f& position, const Vec3f& scale, Vec3f& rotation, const Vec4f& color)
	{
		DrawQuad(modelMat, entityID, { position.X, position.Y, 0.0f }, scale, rotation, color);
	}

	void Renderer2D::DrawQuad(const Mat4f& modelMat, const int entityID,  const Vec3f& position, const Vec3f& scale, Vec3f& rotation, const Vec4f& color)
	{
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		Vec2f textureCoords[4] = { Vec2f( 0.0f, 0.0f ), Vec2f( 1.0f, 0.0f ), Vec2f( 1.0f, 1.0f ), Vec2f( 0.0f, 1.0f ) };
		const float tilingFactor = 1.0f;

		if (s_2DData.QuadIndexCount >= Renderer2DData::MaxQuadIndices)
			FlushAndReset();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
            s_2DData.QuadVertexBufferPtr->EntityID = entityID;
            s_2DData.QuadVertexBufferPtr->Translation = position;
            s_2DData.QuadVertexBufferPtr->Scale = scale;
            s_2DData.QuadVertexBufferPtr->Rotation = rotation;
            s_2DData.QuadVertexBufferPtr->Color = color;
			s_2DData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            // s_2DData.QuadVertexBufferPtr->ModelMat = modelMat;
            s_2DData.QuadVertexBufferPtr->QuadIndexPos = s_2DData.QuadVertexPositions[i];
			s_2DData.QuadVertexBufferPtr++;
		}

		s_2DData.QuadIndexCount += 6;

		s_2DData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Mat4f& modelMat, const int entityID, const Vec2f& position, const Vec3f& scale, Vec3f& rotation, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		DrawQuad(modelMat, entityID, { position.X, position.Y, 0.0f }, scale, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const Mat4f& modelMat, const int entityID,  const Vec3f& position, const Vec3f& scale, Vec3f& rotation, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		constexpr size_t quadVertexCount = 4;

		if (s_2DData.QuadIndexCount >= Renderer2DData::MaxQuadIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_2DData.TextureSlotIndex; i++)
		{
			if (s_2DData.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_2DData.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_2DData.TextureSlotIndex;
			s_2DData.TextureSlots[s_2DData.TextureSlotIndex] = texture;
			s_2DData.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
            s_2DData.QuadVertexBufferPtr->EntityID = entityID;
            s_2DData.QuadVertexBufferPtr->Translation = position;
            s_2DData.QuadVertexBufferPtr->Rotation = rotation;
            s_2DData.QuadVertexBufferPtr->Scale = scale;
			s_2DData.QuadVertexBufferPtr->Color = tintColor;
			s_2DData.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[i];
			s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_2DData.QuadVertexBufferPtr->QuadIndexPos = s_2DData.QuadVertexPositions[i];

            // s_2DData.QuadVertexBufferPtr->ModelMat = modelMat;
			s_2DData.QuadVertexBufferPtr++;
		}

		s_2DData.QuadIndexCount += 6;

		s_2DData.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const Vec3f& p0, const Vec3f& p1, const Vec4f& color)
	{
		if (s_2DData.LineIndexCount >= Renderer2DData::MaxLineIndices)
			FlushAndResetLines();

		s_2DData.LineVertexBufferPtr->Position = p0;
		s_2DData.LineVertexBufferPtr->Color = color;
		s_2DData.LineVertexBufferPtr++;

		s_2DData.LineVertexBufferPtr->Position = p1;
		s_2DData.LineVertexBufferPtr->Color = color;
		s_2DData.LineVertexBufferPtr++;

		s_2DData.LineIndexCount += 2;

		s_2DData.Stats.LineCount++;
	}

	void Renderer2D::ResetStats()
	{
		s_2DData.Stats = Renderer2DStatistics();
	}

	Renderer2DStatistics Renderer2D::GetStats()
	{
		return s_2DData.Stats;
	}
}