#include "NebulaStudioLayer.h"

Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

struct Animation
{
    Vec3f pos = Vec3f(0.0f, 0.0f, 1.0f);
    float animationTime = 1; // Seconds
    float timeSinceStart = 0;
    Vec3f start = pos;
    Vec3f end = Vec3f(2.0f, 0.0f, 0.0f);
    Vec3f velocity = Vec3f((end.x - pos.x) / animationTime, (end.y - pos.y) / animationTime, (end.z - pos.z) / animationTime);
    bool running = false;

    bool reverse = false;

    void LinearCurve(float ts)
    {
        LOG_INF("Linear curve\n");
        if (reverse)
        {
            pos.x = end.x - velocity.x * timeSinceStart;
            pos.y = end.y - velocity.y * timeSinceStart;

        }
        else
        {
            pos.x = start.x + velocity.x * timeSinceStart;
            pos.y = start.y + velocity.y * timeSinceStart;
        }
        

    }

    void Fade(float ts)
    {
        if (reverse)
        {
            pos.z = end.z - (velocity.z * timeSinceStart);
        }
        else
        {
            pos.z = start.z + (velocity.z * timeSinceStart);
        }
        
    }

    void RunAnimation(float ts)
    {
        if (!running && Nebula::Input::IsKeyPressed(Nebula::KEY_SPACE))
        {
            LOG_INF("Ran\n");
            running = true;
        }
        if (!running) return;
        
        timeSinceStart += ts;
        
        if (timeSinceStart >= animationTime)
        {
            running = false;
            reverse = !reverse;
            timeSinceStart = 0;
        }

        LinearCurve(ts);
        Fade(ts);
    }

} BoxAni;

Vec2f BoxPos = Vec2f(0.0, 0.0);

void NebulaStudioLayer::OnAttach()
{
    Nebula::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    FrameBuffer = Nebula::Framebuffer::Create(fbSpec);

    Camera = new Nebula::OrthographicCameraController(fbSpec.Width / fbSpec.Height, true);
    texture = Nebula::Texture2D::Create("Nebula/assets/textures/Missing.png");
}
float tsls = 0.0f;
void NebulaStudioLayer::OnUpdate(float ts)
{
    if (Nebula::FramebufferSpecification spec = FrameBuffer->GetSpecification();
        ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
    {
        FrameBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        Camera->OnResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
    }

    if (m_ViewportFocused)
    {
        Camera->OnUpdate(ts);
    }

    BoxAni.RunAnimation(ts);
    tsls += ts;

    float rotation = 360 * (tsls / 4);

    Nebula::RendererConfig::Clear();
    FrameBuffer->Bind();
    Nebula::RendererConfig::Clear();

    Nebula::Renderer2D::BeginScene(Camera->GetCamera());

    Nebula::Renderer2D::DrawRotatedQuad(Vec3f(BoxAni.pos.x, BoxAni.pos.y, 1.0f), Vec2f(0.5f, 0.5f), rotation * (PI/ 180.0f), Vec4f(0.2, 0.4, 0.9, BoxAni.pos.z));
    // Nebula::Renderer2D::DrawRotatedQuad(Vec3f(0.5, -0.5, 1.0f), Vec2f(0.4, 0.6), 45.0f * (PI/ 180.0f), Vec4f(0.2, 0.4, 0.9, 1.0));

    Nebula::Renderer2D::EndScene();

    FrameBuffer->Unbind();   
}

void NebulaStudioLayer::OnImGuiRender()
{
    ImGui::Begin("Panel");
    ImGui::ColorEdit4("Background", clearColor.elements);
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


void NebulaStudioLayer::OnEvent(Nebula::Event& e)
{
    Camera->OnEvent(e);
}