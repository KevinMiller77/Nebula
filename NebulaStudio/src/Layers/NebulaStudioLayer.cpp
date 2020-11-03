#include "NebulaStudioLayer.h"

namespace Nebula
{
    Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

    Vec2f BoxPos = Vec2f(0.0, 0.0);

    void NebulaStudioLayer::OnAttach()
    {
        //Mount VFS
        if (VFS::Exists(StartProjFileInput, true))
        {
            CurrentProject = StudioProject::LoadProjectFile(StartProjFileInput);
            
        }
        else
        {
            CurrentProject = StudioProject::CreateProjectFile(StartProjFileInput);
        }

        if (VFS::Exists(CurrentProject.LastFileSystemMount, true))
        {
            VFS::Mount(CurrentProject.LastFileSystemMount);
        }
        else
        {
            std::string dir = std::string(CurrentProject.AbsolutePath);

            #ifdef NEB_PLATFORM_WINDOWS
                dir = dir.substr(0, dir.find_last_of("\\") + 1);
            #else
                dir = dir.substr(0, dir.find_last_of("/") + 1);
            #endif

            if (VFS::Exists(dir, true))
            {
                CurrentProject.LastFileSystemMount = dir;
                VFS::Mount(CurrentProject.LastFileSystemMount);
            }
        }


        ActiveScene = CreateRef<Nebula::Scene>();
        if (!CurrentProject.LastSceneOpened.empty())
        {
            SceneSerializer ser(ActiveScene);
            ser.DeserializeTxt(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
            ActiveScene->SetFilePath(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
        }

        Renderer2D::SetShader("assets/shaders/Texture.glsl");

        Autosave.Start();

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1600;
        fbSpec.Height = 900;
        FrameBuffer = Framebuffer::Create(fbSpec);

        PlayStatus = SceneStatus::NOT_STARTED;
        EditorCamera = OrthographicCameraController(ViewportSize.x / ViewportSize.x);

        textures.AddTexture("Missing", VFS::AbsolutePath("assets/textures/Missing.png"));

        // CameraEntity = ActiveScene->CreateEntity("Camera Entity");
        // CameraEntity.AddComponent<CameraComponent>();

        // class GravityOnCube : public ScriptableEntity
        // {
        //     float gravity = 9.81f;
        //     float velocityY;

        //     void OnCreate() override
        //     {
        //         auto& transform = GetComponent<TransformComponent>();
        //         transform.Translation = Vec3f(0.0f, 0.0f, 0.0f);
        //         transform.Rotation = Vec3f(0.0f, 0.0f, 0.0f);
        //         transform.Scale = Vec3f(1.0f, 1.0f, 1.0f);
                 
        //         velocityY = 0.0f;
        //     }

        //     void OnUpdate(float ts) override
        //     {
        //         velocityY += gravity * ts;

        //         auto& transform = GetComponent<TransformComponent>();
        //         transform.Translation.z -= velocityY * ts;
        //     }
        // };

        // auto quad1 = ActiveScene->CreateEntity("First square");
        // quad1.AddComponent<SpriteRendererComponent>(textures.GetTexture("Missing"), Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
        // quad1.AddComponent<NativeScriptComponent>().Bind<GravityOnCube>();

        SceneHierarchy.SetContext(ActiveScene);
        SceneHierarchy.SetTextureLib(&textures);

        FileBrowser.SetTitle("Project Browser");

        
        // ImGuiIO& io = ImGui::GetIO();
        // ImFont* pFont = io.Fonts->AddFontFromFileTTF(VFS::Path("assets/fonts/open-sans/OpenSans-Regular.ttf").c_str(), 18.0f);
        
    }


    float tsls = 0.0f;
    void NebulaStudioLayer::OnUpdate(float ts)
    {
        if (Autosave.GetTimePassed() >= AUTOSAVE_INTERVAL)
        {
            StudioProject::SaveProjectFile(CurrentProject);
            Autosave.Start();
            LOG_INF("Autosaved project\n");
        }

        if (FramebufferSpecification spec = FrameBuffer->GetSpecification();
            // zero sized framebuffer is invalid
            ViewportSize.x > 0.0f && ViewportSize.y > 0.0f && (spec.Width != ViewportSize.x || spec.Height != ViewportSize.y))
        {
            FrameBuffer->Resize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
            EditorCamera.OnResize(ViewportSize.x, ViewportSize.y);
        }

        RendererConfig::Clear();
        FrameBuffer->Bind();
        RendererConfig::Clear();


        if(PlayStatus == SceneStatus::NOT_STARTED)
        {
            ActiveScene->OnEditingUpdate(EditorCamera.GetCamera());
            if(ViewportFocused)
                EditorCamera.OnUpdate(ts);
        }
        else
        {
            ActiveScene->OnUpdate(ts, PlayStatus);
        }

        FrameBuffer->Unbind();   
    }

    void NebulaStudioLayer::OnImGuiRender()
    {
        ImGui::Begin("Controls");
        if(ImGui::Button("Play", ImVec2(25, 25)))
        {
            if (PlayStatus == SceneStatus::NOT_STARTED)
            {
                ActiveScene->OnPlay();
            }
            PlayStatus = SceneStatus::PLAYING;
        }
        ImGui::SameLine();
        if(ImGui::Button("Pause", ImVec2(25, 25)))
        {
            if (PlayStatus == SceneStatus::PLAYING)
            {
                PlayStatus = SceneStatus::PAUSED;
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Stop", ImVec2(25, 25)))
        {
            PlayStatus = SceneStatus::NOT_STARTED;
            ActiveScene->OnStop();
        }
        ImGui::End();

        OnImGuiMenuBar();
        SceneHierarchy.OnImGuiRender();

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
        if (PlayStatus == SceneStatus::NOT_STARTED && ViewportFocused)
        {
            EditorCamera.OnEvent(e);
        }

        EventDispatcher Dispatch(e);
        Dispatch.Dispatch<KeyPressedEvent>(NEB_BIND_EVENT_FN(NebulaStudioLayer::OnKeyPressed));
    }

    bool NebulaStudioLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        bool ctrl = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
        bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

        if (ctrl)
        {
            switch ((KeyCode)e.GetKeyCode())
            {
                case(KeyCode::S):
                {
                    SaveScene();
                    break;
                }

                case(KeyCode::N):
                {
                    NewScene();
                    break;
                }
                case(KeyCode::O):
                {
                    OpenScene();
                    break;
                }

                default:
                    break;
            }
        }

        return false;
    }

    void NebulaStudioLayer::OnImGuiMenuBar()
    {

        if(ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    OpenScene();
                }

                ImGui::Separator();
                
                if (!ActiveScene->GetFilePath().empty())
                {
                    if(ImGui::MenuItem("Save Scene", "Ctrl+S"))
                    {
                        SaveScene();
                    }
                }
                if(ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) 
                {
                    SaveSceneAs();
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
                ImGui::Text("Ver 0.5");
                ImGui::Text("");
                ImGui::Text("Developed by:");
                ImGui::Text("Kevin Miller");

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

    }

    void NebulaStudioLayer::NewScene()
    {
        ActiveScene = CreateRef<Nebula::Scene>();
        ActiveScene->OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
        SceneHierarchy.SetContext(ActiveScene);

        LOG_INF("New scene created.\n");
        SceneHierarchy.ClearSelection();
    }
    
    void NebulaStudioLayer::SaveScene()
    {
        std::string filePath = ActiveScene->GetFilePath();

        if (filePath.empty())
        {
            SaveSceneAs();
            return;
        }
        
        filePath = ReplaceAll(filePath, "\\", "/");
        std::string relPath = VFS::Path(filePath);

        Nebula::SceneSerializer serializer(ActiveScene);
        serializer.SerializeTxt(filePath);

        CurrentProject.LastSceneOpened = relPath;

        LOG_INF("Saved scene!\n");
    }
    
    void NebulaStudioLayer::SaveSceneAs()
    {
        std::string filePath = Nebula::FileDialogs::SaveFile("nst");
        if (!filePath.empty())
        {
            filePath = ReplaceAll(filePath, "\\", "/");
            if (!EndsWith(filePath, {".nst"})) filePath += ".nst";
            
            std::string relPath = VFS::Path(filePath);
            ActiveScene->SetFilePath(relPath);

            Nebula::SceneSerializer serializer(ActiveScene);
            serializer.SerializeTxt(filePath);
            
            CurrentProject.LastSceneOpened = relPath;

            LOG_INF("Saved scene!\n");
            return;
        }
        LOG_INF("Save scene cancelled!\n");
    }
    
    void NebulaStudioLayer::OpenScene()
    {
        std::string filePath = Nebula::FileDialogs::OpenFile("nst");
        if (!filePath.empty())
        {
            filePath = ReplaceAll(filePath, "\\", "/");
            
            std::string relPath = VFS::Path(filePath);

            ActiveScene = CreateRef<Nebula::Scene>();
            ActiveScene->SetFilePath(relPath);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.x, (uint32_t)ViewportSize.y);
            SceneHierarchy.SetContext(ActiveScene);

            Nebula::SceneSerializer serializer(ActiveScene);
            serializer.DeserializeTxt(filePath);

            CurrentProject.LastSceneOpened = relPath;

            LOG_INF("Opened scene!\n");
            return;
        }
        LOG_INF("Open scene cancelled!\n");
        SceneHierarchy.ClearSelection();
    }

}
