#include "Renderer2D.h"
#include "BuiltinShaders.h"
namespace Nebula
{
	static Renderer2DData s_Data;

	void Renderer2D::SetShader(std::string path)
	{
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		//TODO: Material system that handles shaders
		s_Data.TextureShader = Shader::Create("TexQuad", VFS::AbsolutePath(path));
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
	}

	void Renderer2D::ReloadShaders()
	{
		bool TexShaderFileFound = VFS::Exists("assets/shaders/TexQuad.glsl"); 
		if(!TexShaderFileFound)
		{
			LOG_ERR("The shader TexQuad.glsl was not found. It could not be reloaded.\n");
			LOG_ERR("Press Shift+F6 to load default shaders\n");
		}
		else
		{
			s_Data.TextureShader->Reload(VFS::AbsolutePath("assets/shaders/TexQuad.glsl"), true);
			s_Data.TextureShader->Bind();

			int32_t samplers[s_Data.MaxTextureSlots];
			for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
				samplers[i] = i;
			s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
		}
		
		bool LineShaderFileFound = VFS::Exists("assets/shaders/Line.glsl");
		if (!LineShaderFileFound)
		{
			LOG_ERR("The shader Line.glsl was not found. It could not be reloaded.\n");
			LOG_ERR("Press Shift+F6 to load default shaders\n");
		}
		else
		{
			s_Data.LineShader->Reload(VFS::AbsolutePath("assets/shaders/Line.glsl"), true);
		}
	}

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxQuadVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
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
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxQuadVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxQuadIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxQuadIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_Data.QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxQuadIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		bool TexShaderFileFound = VFS::Exists("assets/shaders/TexQuad.glsl"); 
		s_Data.TextureShader =  TexShaderFileFound ? Shader::Create("TexQuad", "assets/shaders/TexQuad.glsl") : Shader::Create("TexQuad", Builtin::TexQuad);
		if (!TexShaderFileFound) LOG_WRN("The shader TexQuad.glsl was not found, the default shader will be used\n");

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set first texture slot to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		bool LineShaderFileFound = VFS::Exists("assets/shaders/Line.glsl");
		s_Data.LineShader = LineShaderFileFound  ? Shader::Create("Line", "assets/shaders/TexQuad.glsl") : Shader::Create("Line", Builtin::Line);
		if(!LineShaderFileFound) LOG_WRN("The shader Line.glsl was not found, the default shader will be used\n");
		
		s_Data.LineVertexArray = VertexArray::Create();
			
		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxLineVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxLineVertices];

		uint32_t* lineIndicies = new uint32_t[s_Data.MaxLineIndices];
		for (uint32_t i = 0; i < s_Data.MaxLineIndices; i++)
		{
			lineIndicies[i] = i;
		}
		s_Data.LineIndexBuffer = IndexBuffer::Create(lineIndicies, s_Data.MaxLineIndices);
		delete[] lineIndicies;
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;
	}

	void Renderer2D::BeginScene(Camera& camera, Mat4f transform)
	{
		ResetStats();

		s_Data.viewProj = camera.GetProjection() * transform.invertMatrix();

		if (s_Data.TextureShader)
		{
			s_Data.TextureShader.get()->Bind();
			s_Data.TextureShader.get()->SetMat4("u_ViewProjection", s_Data.viewProj);
		}

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

    void Renderer2D::BeginScene(EditorCamera& camera)
	{
		ResetStats();
        
        Renderer2D::DrawLine({-50000, 0, 0}, {50000, 0, 0}, {1.0, 1.0, 1.0, 1.0});

		s_Data.viewProj = camera.GetViewProjection();

		if (s_Data.TextureShader)
		{
			s_Data.TextureShader.get()->Bind();
			s_Data.TextureShader.get()->SetMat4("u_ViewProjection", s_Data.viewProj);
		}

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(Mat4f viewProj)
	{
		ResetStats();
        
		s_Data.viewProj = viewProj;

		if (s_Data.TextureShader)
		{
			s_Data.TextureShader->Bind();
			s_Data.TextureShader->SetMat4("u_ViewProjection", s_Data.viewProj);
		}

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		// Quads
		uint32_t data = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		if (data)
		{
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, data);
			
			s_Data.TextureShader->Bind();
			s_Data.TextureShader->SetMat4("u_ViewProjection", s_Data.viewProj);

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);
			
			s_Data.QuadVertexArray->Bind();
			s_Data.QuadIndexBuffer->Bind();

			RendererConfig::DrawIndexed(s_Data.QuadVertexArray, PrimativeType::TRIANGLES, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
		//Lines
		data = (uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase;
		if (data)
		{
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, data);

			s_Data.LineShader->Bind();
			s_Data.LineShader->SetMat4("u_ViewProjection", s_Data.viewProj);

			s_Data.LineVertexArray->Bind();
			s_Data.LineIndexBuffer->Bind();
			
			RendererConfig::DrawIndexed(s_Data.LineVertexArray, PrimativeType::LINES, s_Data.LineIndexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::Flush()
	{}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
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

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxQuadIndices)
			FlushAndReset();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
            s_Data.QuadVertexBufferPtr->EntityID = entityID;
            s_Data.QuadVertexBufferPtr->Translation = position;
            s_Data.QuadVertexBufferPtr->Scale = scale;
            s_Data.QuadVertexBufferPtr->Rotation = rotation;
            s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            // s_Data.QuadVertexBufferPtr->ModelMat = modelMat;
            s_Data.QuadVertexBufferPtr->QuadIndexPos = s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Mat4f& modelMat, const int entityID, const Vec2f& position, const Vec3f& scale, Vec3f& rotation, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		DrawQuad(modelMat, entityID, { position.X, position.Y, 0.0f }, scale, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const Mat4f& modelMat, const int entityID,  const Vec3f& position, const Vec3f& scale, Vec3f& rotation, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxQuadIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
            s_Data.QuadVertexBufferPtr->EntityID = entityID;
            s_Data.QuadVertexBufferPtr->Translation = position;
            s_Data.QuadVertexBufferPtr->Rotation = rotation;
            s_Data.QuadVertexBufferPtr->Scale = scale;
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr->QuadIndexPos = s_Data.QuadVertexPositions[i];

            // s_Data.QuadVertexBufferPtr->ModelMat = modelMat;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
/*
	void Renderer2D::DrawQuad(const Mat4f& transform, const Vec4f& color)
	{
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		Vec2f textureCoords[4] = { Vec2f( 0.0f, 0.0f ), Vec2f( 1.0f, 0.0f ), Vec2f( 1.0f, 1.0f ), Vec2f( 0.0f, 1.0f ) };
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxQuadIndices)
			FlushAndReset();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
            Vec4f tempPos = Vec4f(transform * s_Data.QuadVertexPositions[i]);
            s_Data.QuadVertexBufferPtr->Position = { tempPos.X, tempPos.Y, tempPos.Z };
            s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Mat4f& transform, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxQuadIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
            Vec4f tempPos = Vec4f(transform * s_Data.QuadVertexPositions[i]);
            s_Data.QuadVertexBufferPtr->Position = { tempPos.X, tempPos.Y, tempPos.Z };
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const Vec2f& position, const Vec2f& size, float rotation, const Vec4f& color)
	{
		DrawRotatedQuad({ position.X, position.Y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const Vec3f& position, const Vec2f& size, float rotation, const Vec4f& color)
	{
		Mat4f translation = Mat4f::translation(position);
		Mat4f scale = Mat4f::scale(Vec3f(size.X, size.Y, 1.0f));
		Mat4f rot = Mat4f::rotation(rotation, Vec3f(0,0,1));
		Mat4f transform = rot * scale * translation;

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const Vec2f& position, const Vec2f& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		DrawRotatedQuad({ position.X, position.Y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const Vec3f& position, const Vec2f& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const Vec4f& tintColor)
	{
		Mat4f translation = Mat4f::translation(position);
		Mat4f scale = Mat4f::scale(Vec3f(size.X, size.Y, 1.0f));
		Mat4f rot = Mat4f::rotation(rotation, Vec3f(0,0,1));
		Mat4f transform = rot * scale * translation;

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}
*/
	void Renderer2D::DrawLine(const Vec3f& p0, const Vec3f& p1, const Vec4f& color)
	{
		if (s_Data.LineIndexCount >= Renderer2DData::MaxLineIndices)
			FlushAndResetLines();

		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineIndexCount += 2;

		s_Data.Stats.LineCount++;
	}

	void Renderer2D::ResetStats()
	{
		s_Data.Stats = Renderer2DStatistics();
	}

	Renderer2DStatistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}