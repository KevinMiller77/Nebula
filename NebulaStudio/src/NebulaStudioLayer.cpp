#include "NebulaStudioLayer.h"

namespace Nebula
{
    Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

    Vec2f BoxPos = Vec2f(0.0, 0.0);

    void NebulaStudioLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        FrameBuffer = Framebuffer::Create(fbSpec);

        texture = Texture2D::Create("assets/textures/Missing.png");

        CameraEntity = Scene.CreateEntity("Camera Entity");
        CameraEntity.AddComponent<CameraComponent>();

        class CameraController : public ScriptableEntity
        {
        public:
            virtual void OnCreate() override
            {
                auto& transform = GetComponent<TransformComponent>();
                transform.Translation.x = 0.0f;

                LOG_INF("This is some text\n");
            }

            virtual void OnDestroy() override
            {
            }

            virtual void OnUpdate(float ts) override
            {
                auto& transform = GetComponent<TransformComponent>();

                float speed = 5.0f;

                if (Input::IsKeyPressed(KeyCode::A))
                    transform.Translation.x -= speed * ts;
                if (Input::IsKeyPressed(KeyCode::D))
                    transform.Translation.x += speed * ts;
                if (Input::IsKeyPressed(KeyCode::W))
                    transform.Translation.y += speed * ts;
                if (Input::IsKeyPressed(KeyCode::S))
                    transform.Translation.y -= speed * ts;
            }
        };

        CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        auto quad1 = Scene.CreateEntity("First square");
        quad1.AddComponent<SpriteRendererComponent>(texture, Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

        SceneHierarchyPanel.SetContext(&Scene);
        Log = LogPanel();
    }
    float tsls = 0.0f;
    void NebulaStudioLayer::OnUpdate(float ts)
    {
        if (FramebufferSpecification spec = FrameBuffer->GetSpecification();
            // zero sized framebuffer is invalid
            ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && (spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
        {
            FrameBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
            Scene.OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        }


        RendererConfig::Clear();
        FrameBuffer->Bind();
        RendererConfig::Clear();

        Scene.OnUpdate(ts);

        FrameBuffer->Unbind();   
    }

    void NebulaStudioLayer::OnImGuiRender()
    {
        SceneHierarchyPanel.OnImGuiRender();
        bool t = true;
        Log.Update(t);

        ImGui::Begin("Stats");
        
        Renderer2D::Statistics stats = Renderer2D::GetStats();
        
        ImGui::Text("Draw Calls     : %d", stats.DrawCalls);    
        ImGui::Text("Quad Count     : %d", stats.QuadCount);    
        ImGui::Text("Vertex Count   : %d ", stats.GetTotalVertexCount());    
        ImGui::Text("Index Count    : %d", stats.GetTotalIndexCount());

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        ViewportFocused = ImGui::IsWindowFocused();
        ViewportHovered = ImGui::IsWindowHovered();
        Application::Get()->GetImGuiLayer()->BlockEvents(!ViewportFocused || !ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint64_t textureID = FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ ViewportSize.x, ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        
        
        ImGui::End();
        ImGui::PopStyleVar();
    }


    void NebulaStudioLayer::OnEvent(Event& e)
    {
    }
}