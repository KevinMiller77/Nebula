#include "Renderer.h"
#include "RendererAPI.h"

namespace Nebula{
    Renderer::SceneData_t* Renderer::SceneData = new Renderer::SceneData_t;
    Ref<ShaderLibrary> Renderer::s_ShaderLibrary = nullptr;
    struct RendererData
	{
		Ref<RenderPass> m_ActiveRenderPass;
		RenderCommandQueue m_CommandQueue;
		// Ref<ShaderLibrary> m_ShaderLibrary;

		Ref<VertexBuffer> m_FullscreenQuadVertexBuffer;
		Ref<IndexBuffer> m_FullscreenQuadIndexBuffer;
		Ref<Pipeline> m_FullscreenQuadPipeline;
	};

	static RendererData s_Data;

	std::vector<std::string> Renderer2DShaders = {"TexQuad", "Line"};
    void Renderer::Init()
	{
        RendererConfig::Init();

        s_ShaderLibrary = CreateRef<ShaderLibrary>();

		// 2D Textures (!!! DO NOT RELOAD THESE FROM THE 3D RENDERER !!!)
        s_ShaderLibrary->Load(VFS::AbsolutePath("assets/shaders/Line.glsl"));
        s_ShaderLibrary->Load(VFS::AbsolutePath("assets/shaders/TexQuad.glsl"));

        // Add all shaders to the renderer lib here
        s_ShaderLibrary->Load(VFS::AbsolutePath("assets/shaders/PBR_Static.glsl"));

		Renderer2D::Init();
    }
    void Renderer::Shutdown()
	{
        Renderer2D::Shutdown();
	}

    void Renderer::SubmitQuad(Ref<MaterialInstance> material, const Mat4f& transform)
	{
		bool depthTest = true;
		if (material)
		{
			material->Bind();
			depthTest = material->GetFlag(MaterialFlag::DepthTest);

			auto shader = material->GetShader();
			shader->SetUniformBuffer("Transform", &transform, sizeof(Vec4f) * 4);
		}

		s_Data.m_FullscreenQuadVertexBuffer->Bind();
		s_Data.m_FullscreenQuadPipeline->Bind();
		s_Data.m_FullscreenQuadIndexBuffer->Bind();
		RendererConfig::DrawIndexed(6, PrimativeType::TRIANGLES, depthTest);
	}

	void Renderer::SubmitFullscreenQuad(Ref<MaterialInstance> material)
	{
		bool depthTest = true;
		if (material)
		{
			material->Bind();
			depthTest = material->GetFlag(MaterialFlag::DepthTest);
		}

		s_Data.m_FullscreenQuadVertexBuffer->Bind();
		s_Data.m_FullscreenQuadPipeline->Bind();
		s_Data.m_FullscreenQuadIndexBuffer->Bind();
		RendererConfig::DrawIndexed(6, PrimativeType::TRIANGLES, depthTest);
	}

	void Renderer::SubmitMesh(Ref<Mesh> mesh, const Mat4f& transform, Ref<MaterialInstance> overrideMaterial)
	{
		// TODO: Sort this out
		// auto material = overrideMaterial ? overrideMaterial : mesh->GetMaterialInstance();
		// auto shader = material->GetShader();

		int entityID = mesh->GetEntityID();

		mesh->m_VertexBuffer->Bind();
		mesh->m_Pipeline->Bind();
		mesh->m_IndexBuffer->Bind();

		auto& materials = mesh->GetMaterials();
		for (Submesh& submesh : mesh->m_Submeshes)
		{
			// Material
			auto material = overrideMaterial ? overrideMaterial : materials[submesh.MaterialIndex];
			auto shader = material->GetShader();
			material->Bind();

			if (false && mesh->m_IsAnimated)
			{
				for (size_t i = 0; i < mesh->m_BoneTransforms.size(); i++)
				{
					std::string uniformName = std::string("u_BoneTransforms[") + std::to_string(i) + std::string("]");
					mesh->m_MeshShader->SetUniform(uniformName, mesh->m_BoneTransforms[i]);
				}
			}

			shader->SetUniformBuffer("Entity", &entityID, sizeof(int));
			
			auto transformUniform = transform * submesh.Transform;
			shader->SetUniformBuffer("Transform", &transformUniform, sizeof(Vec4f) * 4);

            if (material->GetFlag(MaterialFlag::DepthTest))	
                RendererConfig::SetDepthTest(true);
            else
                RendererConfig::SetDepthTest(false);

            RendererConfig::DrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, 0, submesh.BaseVertex);
		}
	}

    void Renderer::DrawAABB(Ref<Mesh> mesh, const Mat4f& transform, const Vec4f& color)
	{
		for (Submesh& submesh : mesh->m_Submeshes)
		{
			auto& aabb = submesh.BoundingBox;
			auto aabbTransform = transform * submesh.Transform;
			DrawAABB(aabb, aabbTransform);
		}
	}

	void Renderer::DrawAABB(const AABB& aabb, const Mat4f& transform, const Vec4f& color /*= Vec4f(1.0f)*/)
	{
		Vec4f min = { aabb.Min.X, aabb.Min.Y, aabb.Min.Z, 1.0f };
		Vec4f max = { aabb.Max.X, aabb.Max.Y, aabb.Max.Z, 1.0f };

		Vec4f corners[8] =
		{
			transform * Vec4f { aabb.Min.X, aabb.Min.Y, aabb.Max.Z, 1.0f },
			transform * Vec4f { aabb.Min.X, aabb.Max.Y, aabb.Max.Z, 1.0f },
			transform * Vec4f { aabb.Max.X, aabb.Max.Y, aabb.Max.Z, 1.0f },
			transform * Vec4f { aabb.Max.X, aabb.Min.Y, aabb.Max.Z, 1.0f },

			transform * Vec4f { aabb.Min.X, aabb.Min.Y, aabb.Min.Z, 1.0f },
			transform * Vec4f { aabb.Min.X, aabb.Max.Y, aabb.Min.Z, 1.0f },
			transform * Vec4f { aabb.Max.X, aabb.Max.Y, aabb.Min.Z, 1.0f },
			transform * Vec4f { aabb.Max.X, aabb.Min.Y, aabb.Min.Z, 1.0f }
		};

		for (uint32_t i = 0; i < 4; i++)
			Renderer2D::DrawLine(corners[i].XYZ(), corners[(i + 1) % 4].XYZ(), color);

		for (uint32_t i = 0; i < 4; i++)
			Renderer2D::DrawLine(corners[i + 4].XYZ(), corners[((i + 1) % 4) + 4].XYZ(), color);

		for (uint32_t i = 0; i < 4; i++)
			Renderer2D::DrawLine(corners[i].XYZ(), corners[i + 4].XYZ(), color);
	}


    void Renderer::OnWindowResize(uint32 w, uint32 h)
	{
        RendererConfig::SetViewport(0, 0, w, h);
	}

    void Renderer::BeginRenderPass(Ref<RenderPass> renderPass, bool clear) {
        s_Data.m_ActiveRenderPass = renderPass;

        renderPass->GetSpecification().TargetFramebuffer->Bind();
        if (clear) {
            Vec4f clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
            //Renderer::Submit([=]() {
                RendererConfig::Clear(clearColor);
            //});
        }
	}

    void Renderer::EndRenderPass()
	{
		assert(s_Data.m_ActiveRenderPass, "No active render pass! Have you called Renderer::EndRenderPass twice?");
		s_Data.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
		s_Data.m_ActiveRenderPass = nullptr;
	}

	
    void Renderer::ReloadShaders()
	{
		for (std::pair<std::string, Ref<Shader>> shader : s_ShaderLibrary->GetAll()) {
			// If its a 2D shader, don't handle it here
			if (std::find(Renderer2DShaders.begin(), Renderer2DShaders.end(), shader.first) != Renderer2DShaders.end()) {
				continue;
			}
			
			shader.second->Reload(true);
		}

		Renderer2D::ReloadShaders();

		LOG_INF("Shaders Reloaded\n");
	}

    
    void Renderer::WaitAndRender() {
        s_Data.m_CommandQueue.Execute();
    }
    
    RenderCommandQueue& Renderer::GetRenderCommandQueue() {
        return s_Data.m_CommandQueue;
    }

	   
}