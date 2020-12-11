#include "Renderer.h"

namespace Nebula
{
    Renderer::SceneData_t* Renderer::SceneData = new Renderer::SceneData_t;

    void Renderer::Init()
	{
        RendererConfig::Init();
		Renderer2D::Init();
    }
    void Renderer::Shutdown()
	{
        Renderer2D::Shutdown();
	}

    void Renderer::OnWindowResize(uint32 w, uint32 h)
	{
        RendererConfig::SetViewport(0, 0, w, h);
	}

    void Renderer::BeginScene(Camera camera)
	{
        SceneData->ViewProjMatrix = camera.GetViewProjection();
	}

    void Renderer::EndScene()
	{
		
	}

	
    void Renderer::ReloadShaders()
	{
		Renderer2D::ReloadShaders();

		LOG_INF("Shaders Reloaded\n");
	}

    void Renderer::Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const Mat4f transform)
	{
        shader->Bind();
		shader->SetMat4("u_ViewProjection", SceneData->ViewProjMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RendererConfig::DrawIndexed(vertexArray, PrimativeType::TRIANGLES);
	}

	   
}