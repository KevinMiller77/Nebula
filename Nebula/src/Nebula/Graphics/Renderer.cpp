#include "Renderer.h"

namespace Nebula
{
    Renderer::SceneData_t* Renderer::SceneData = new Renderer::SceneData_t;

    void Renderer::Init()
	{
        RendererConfig::Init();
    //Init 2D and 3D renderers
    }
    void Renderer::Shutdown()
	{
        //Shut down 2D and 3D renderer
	}

    void Renderer::OnWindowResize(unsigned int w, unsigned int h)
	{
        RendererConfig::SetViewport(0, 0, w, h);
	}

    void Renderer::BeginScene(Camera camera)
	{
        SceneData->ViewProjMatrix = camera.GetProjection();
	}

    void Renderer::EndScene()
	{

	}

    void Renderer::Submit(Shader* shader, VertexArray* vertexArray, const Mat4f transform)
	{
        shader->Bind();
		shader->SetMat4("u_ViewProjection", SceneData->ViewProjMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RendererConfig::DrawIndexed(vertexArray);
	}
    
}