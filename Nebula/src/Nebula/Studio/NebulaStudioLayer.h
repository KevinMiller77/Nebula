#pragma once
#include <Nebula.h>

static Vec4f clearColor = {0.1f, 0.1f, 0.1f, 1.0f};
class NebulaStudioLayer : public Nebula::Layer
{
public:
    float vertexData[7 * 3] = 
    {
        0.5f, -0.5f, 0.0f, 0.41f, 0.0f, 0.18f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 0.99f, 0.2f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f
    };
    float vertexData2[7 * 3] = 
    {
        -0.5f, 0.5f, 0.0f, 0.41f, 0.0f, 0.18f, 1.0f,
        0.0f, -0.5f, 0.0f, 0.7f, 0.99f, 0.2f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.9f, 0.5f, 0.5f, 1.0f
    };

    unsigned int indicies[3] = {0, 1, 2};

    Nebula::Shader* shader;
    Nebula::VertexArray* tri1;    
    Nebula::VertexArray* tri2;


    Nebula::BufferLayout layout =  {
        {Nebula::ShaderDataType::Float3, "position"},
        {Nebula::ShaderDataType::Float4, "color"}
    };

    Nebula::Framebuffer* FrameBuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    Vec2f ViewportSize = {0.0f, 0.0f};

    NebulaStudioLayer() = default;
    
    void OnAttach() override
    {
        Nebula::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		FrameBuffer = Nebula::Framebuffer::Create(fbSpec);


        tri1 = Nebula::VertexArray::Create();
        tri2 = Nebula::VertexArray::Create();
        
        auto vb = Nebula::VertexBuffer::Create(vertexData, sizeof(vertexData));
        vb->SetLayout(layout);
        tri1->AddVertexBuffer(vb);

        tri1->SetIndexBuffer(Nebula::IndexBuffer::Create(indicies, sizeof(indicies)));

        auto vb2 = Nebula::VertexBuffer::Create(vertexData2, sizeof(vertexData2));
        vb2->SetLayout(layout);
        tri2->AddVertexBuffer(vb2);

        tri1->SetIndexBuffer(Nebula::IndexBuffer::Create(indicies, sizeof(indicies)));
        tri2->SetIndexBuffer(Nebula::IndexBuffer::Create(indicies, sizeof(indicies)));

        shader = Nebula::Shader::Create("Nebula/assets/shaders/FlatColor.glsl");



    }

    void OnUpdate(float ts) override
    {
        if (Nebula::FramebufferSpecification spec = FrameBuffer->GetSpecification();
			ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
		{
			FrameBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
		}


        Nebula::RendererConfig::Clear();
        FrameBuffer->Bind();
        Nebula::RendererConfig::Clear();


        Nebula::Renderer::BeginScene(Nebula::Camera(Mat4f(1.0f)));


        Nebula::Renderer::Submit(shader, tri2);
        Nebula::Renderer::Submit(shader, tri1);

        Nebula::Renderer::EndScene();

        FrameBuffer->Unbind();   
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Panel");
        ImGui::ColorPicker4("Background", clearColor.elements);
        ImGui::Text("This probably looks weird as fuck");
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Nebula::Application::Get()->GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ ViewportSize.x, ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
    }
};