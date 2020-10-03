#include "NebulaStudioLayer.h"

Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

Vec2f BoxPos = Vec2f(0.0, 0.0);

void NebulaStudioLayer::OnAttach()
{
    Nebula::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    FrameBuffer = Nebula::Framebuffer::Create(fbSpec);

    Camera = new Nebula::OrthographicCameraController((float)fbSpec.Width / (float)fbSpec.Height);
    texture = Nebula::Texture2D::Create("assets/textures/Missing.png");
    
    quads.push_back(new QuadInfo("Textured", texture));
    quads.push_back(new QuadInfo("No Texture"));
    quads[1]->Pos = Vec3f(-1.0f, 1.0f, 0.0f);
    quads[1]->Color = Vec4f(0.2f, 0.5f, 0.5f, 1.0f);
}
float tsls = 0.0f;
void NebulaStudioLayer::OnUpdate(float ts)
{
    if (Nebula::FramebufferSpecification spec = FrameBuffer->GetSpecification();
        ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
    {
        FrameBuffer->Resize((uint32)ViewportSize.x, (uint32)ViewportSize.y);
        Camera->OnResize(ViewportSize.x, ViewportSize.y);
    }

    if (m_ViewportFocused)
    {
        Camera->OnUpdate(ts);
    }

    Nebula::RendererConfig::Clear();
    FrameBuffer->Bind();
    Nebula::RendererConfig::Clear();

    Nebula::Renderer2D::BeginScene(Camera->GetCamera());

    for (QuadInfo* quad : quads)
    {
        if (quad->Texture != nullptr)
        {
            quad->Rotation != 0 ? Nebula::Renderer2D::DrawRotatedQuad(quad->Pos, quad->Size, quad->Rotation, quad->Texture, 2.0f, quad->Color) : Nebula::Renderer2D::DrawQuad(quad->Pos, quad->Size, quad->Texture, 1.0f, quad->Color);
        }
        else
        {
            quad->Rotation != 0 ? Nebula::Renderer2D::DrawRotatedQuad(quad->Pos, quad->Size, quad->Rotation, quad->Color) : Nebula::Renderer2D::DrawQuad(quad->Pos, quad->Size, quad->Color);
        }
        
    }
    Nebula::Renderer2D::EndScene();

    FrameBuffer->Unbind();   
}

void NebulaStudioLayer::OnImGuiRender()
{
    ImGui::Begin("Props");
    ImGui::Text("These are your quads");
    for (int i = 0; i < 2; i++)
    {
        QuadInfo* quad = quads[i];

        ImGui::PushID(std::string(quad->Name).c_str());

        ImGui::Separator();
        ImGui::Text("Quad: %s", quad->Name);
        ImGui::SliderFloat3("Pos", quad->Pos.elements, -2.0f, 2.0f);
        ImGui::SliderFloat2("Size", quad->Size.elements, 0.1f, 2.0f);
        ImGui::SliderAngle("Rotation", &(quad->Rotation));
        ImGui::ColorEdit4("Color", quad->Color.elements);        

        ImGui::PopID();
    }
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


void NebulaStudioLayer::OnEvent(Nebula::Event& e)
{
    Camera->OnEvent(e);
}