#include "NebulaStudioLayer.h"

Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

Vec2f BoxPos = Vec2f(0.0, 0.0);

void NebulaStudioLayer::OnAttach()
{
    Nebula::FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    FrameBuffer = Nebula::Framebuffer::Create(fbSpec);

    texture = Nebula::Texture2D::Create("assets/textures/Missing.png");
    
    // quads.push_back(new QuadInfo("Textured", texture));
    // quads.push_back(new QuadInfo("No Texture"));
    // quads[1]->Pos = Vec3f(-1.0f, 1.0f, 0.0f);
    // quads[1]->Color = Vec4f(0.2f, 0.5f, 0.5f, 1.0f);

    CameraEntity = Scene.CreateEntity("Camera Entity");
    CameraEntity.AddComponent<Nebula::CameraComponent>();

    class CameraController : public Nebula::ScriptableEntity
    {
    public:
        virtual void OnCreate() override
        {
            auto& transform = GetComponent<Nebula::TransformComponent>().Transform;
            transform[3][0] = rand() % 10 - 5.0f;
        }

        virtual void OnDestroy() override
        {
        }

        virtual void OnUpdate(float ts) override
        {
            auto& transform = GetComponent<Nebula::TransformComponent>().Transform;

            float speed = 5.0f;

            if (Nebula::Input::IsKeyPressed(Nebula::KeyCode::A))
                transform[3][0] -= speed * ts;
            if (Nebula::Input::IsKeyPressed(Nebula::KeyCode::D))
                transform[3][0] += speed * ts;
            if (Nebula::Input::IsKeyPressed(Nebula::KeyCode::W))
                transform[3][1] += speed * ts;
            if (Nebula::Input::IsKeyPressed(Nebula::KeyCode::S))
                transform[3][1] -= speed * ts;
        }
    };

    CameraEntity.AddComponent<Nebula::NativeScriptComponent>().Bind<CameraController>();

    auto quad1 = Scene.CreateEntity("First square");
    quad1.AddComponent<Nebula::SpriteRendererComponent>(texture, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

    SceneHierarchyPanel.SetContext(&Scene);
}
float tsls = 0.0f;
void NebulaStudioLayer::OnUpdate(float ts)
{
    if (Nebula::FramebufferSpecification spec = FrameBuffer->GetSpecification();
        ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
    {
        FrameBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        Scene.OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
    }


    Nebula::RendererConfig::Clear();
    FrameBuffer->Bind();
    Nebula::RendererConfig::Clear();

    Scene.OnUpdate(ts);

    FrameBuffer->Unbind();   
}

void NebulaStudioLayer::OnImGuiRender()
{
    SceneHierarchyPanel.OnImGuiRender();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");

    ViewportFocused = ImGui::IsWindowFocused();
    ViewportHovered = ImGui::IsWindowHovered();
    Nebula::Application::Get()->GetImGuiLayer()->BlockEvents(!ViewportFocused || !ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    uint64_t textureID = FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ ViewportSize.x, ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    
    ImGui::End();
    ImGui::PopStyleVar();
}


void NebulaStudioLayer::OnEvent(Nebula::Event& e)
{
}