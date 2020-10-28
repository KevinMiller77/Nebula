#include "NebulaStudioLayer.h"

namespace Nebula
{
    Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

    Vec2f BoxPos = Vec2f(0.0, 0.0);

    void NebulaStudioLayer::OnAttach()
    {
        ActiveScene = CreateRef<Nebula::Scene>();

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        FrameBuffer = Framebuffer::Create(fbSpec);

        textures.AddTexture("Missing", "assets/textures/Missing.png");

        CameraEntity = ActiveScene->CreateEntity("Camera Entity");
        CameraEntity.AddComponent<CameraComponent>();

        // class CameraController : public ScriptableEntity
        // {
        // public:
        //     virtual void OnCreate() override
        //     {
        //         auto& transform = GetComponent<TransformComponent>();
        //         transform.Translation.x = 0.0f;
        //     }

        //     virtual void OnDestroy() override
        //     {
        //     }

        //     virtual void OnUpdate(float ts) override
        //     {
        //         auto& transform = GetComponent<TransformComponent>();

        //         float speed = 5.0f;

        //         if (Input::IsKeyPressed(KeyCode::A))
        //             transform.Translation.x -= speed * ts;
        //         if (Input::IsKeyPressed(KeyCode::D))
        //             transform.Translation.x += speed * ts;
        //         if (Input::IsKeyPressed(KeyCode::W))
        //             transform.Translation.y += speed * ts;
        //         if (Input::IsKeyPressed(KeyCode::S))
        //             transform.Translation.y -= speed * ts;
        //     }
        // };

        // CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        auto quad1 = ActiveScene->CreateEntity("First square");
        quad1.AddComponent<SpriteRendererComponent>(textures.GetTexture("Missing"), Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

        SceneHierarchyPanel.SetContext(ActiveScene);
        SceneHierarchyPanel.SetTextureLib(&textures);

        FileBrowser.SetTitle("Project Browser");
    }


    float tsls = 0.0f;
    void NebulaStudioLayer::OnUpdate(float ts)
    {
        if (FramebufferSpecification spec = FrameBuffer->GetSpecification();
            // zero sized framebuffer is invalid
            ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && (spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
        {
            FrameBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        }


        RendererConfig::Clear();
        FrameBuffer->Bind();
        RendererConfig::Clear();

        ActiveScene->OnUpdate(ts);

        FrameBuffer->Unbind();   
    }

    

    void NebulaStudioLayer::OnImGuiRender()
    {
        OnImGuiMenuBar();
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

    void NebulaStudioLayer::OnImGuiMenuBar()
    {

        if(ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    ActiveScene = CreateRef<Nebula::Scene>();
                    ActiveScene->OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
                    SceneHierarchyPanel.SetContext(ActiveScene);
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    std::string filePath = Nebula::FileDialogs::OpenFile("Nebula Scene (*.NebScene)\0*.NebScene\0");
                    if (!filePath.empty())
                    {
                        ActiveScene = CreateRef<Nebula::Scene>();
                        ActiveScene->SetFilePath(filePath);
                        ActiveScene->OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
                        SceneHierarchyPanel.SetContext(ActiveScene);

                        Nebula::SceneSerializer serializer(ActiveScene);
                        serializer.DeserializeTxt(filePath);
                    }
                }

                ImGui::Separator();
                
                if (!ActiveScene->GetFilePath().empty())
                {
                    if(ImGui::MenuItem("Save", "Ctrl+S"))
                    {
                        std::string filePath = ActiveScene->GetFilePath();
                        Nebula::SceneSerializer serializer(ActiveScene);
                        serializer.SerializeTxt(filePath);
                        //TODO: Serialize ya shit
                    }
                }
                if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) 
                {
                    
                    std::string filePath = Nebula::FileDialogs::SaveFile("Nebula Scene (*.NebScene)\0*.NebScene\0");
                    if (!filePath.empty())
                    {
                        if (!EndsWith(filePath, {".NebScene"})) filePath += ".NebScene";

                        ActiveScene->SetFilePath(filePath);
                        Nebula::SceneSerializer serializer(ActiveScene);
                        serializer.SerializeTxt(filePath);
                    }
                }

                if (ImGui::MenuItem("Exit")) 
                {
                    Application::Get()->Close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About"))
            {
                ImGui::Text("Nebula Studio");
                ImGui::Text("Ver 0.0.5");
                ImGui::Text("");
                ImGui::Text("Developed by;");
                ImGui::Text("Kevin Miller");

                ImGui::EndMenu();
            }


            // float ButWidth = 45.0f;
            // ImGui::SameLine();

            // ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ButWidth * 3.0f);
            // if(ImGui::Button("_", ImVec2(ButWidth, ImGui::GetFrameHeight())))
            // {
            //     Application::Get()->GetWindow()->MinimizeWindow();
            // }
            // ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ButWidth * 2.0f);
            // if (Application::Get()->GetWindow()->IsMaximized())
            // {
            //     if(ImGui::Button("v", ImVec2(ButWidth, ImGui::GetFrameHeight())))
            //     {  
            //         Application::Get()->GetWindow()->RestoreWindow();
            //     }
            // }
            // else
            // {
            //     if(ImGui::Button("^", ImVec2(ButWidth, ImGui::GetFrameHeight())))
            //     {  
            //         Application::Get()->GetWindow()->MaximizeWindow();
            //     }
            // }
            // ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ButWidth);
            // if(ImGui::Button("X", ImVec2(ButWidth, ImGui::GetFrameHeight())))
            // {
            //     Application::Get()->Close();
            // }

            ImGui::EndMenuBar();
        }
    }

}