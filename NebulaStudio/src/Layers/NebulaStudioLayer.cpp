#include "NebulaStudioLayer.h"

namespace Nebula
{
    Vec4f NebulaStudioLayer::clearColor = {0.1f, 0.1f, 0.1f, 1.0f};

    Vec2f BoxPos = Vec2f(0.0, 0.0);

    // Happens every time you open a project

    void NebulaStudioLayer::OnAttach()
    {
        OpenProject(ProjFileInput);

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1600;
        fbSpec.Height = 900;
        fbSpec.Samples = 1;
        FrameBuffer = Framebuffer::Create(fbSpec);

        AudioSource as = Audio::LoadAudioSource(VFS::AbsolutePath("assets/sounds/HeadFirst.ogg"));
        as.SetLoop(true);
        as.SetGain(0.5f);
        as.SetSpatial(true);

        Audio::Play(as);
    }

    float tsls = 0.0f;
    void NebulaStudioLayer::OnUpdate(float ts)
    {
        if (ClipboardFull && !Clipboard.IsValid())
        {
            ClipboardFull = false;
            Clipboard = Entity();
        }

        if (Autosave.GetTimePassed() >= AUTOSAVE_INTERVAL)
        {
            SaveProject(CurrentProject);
            Autosave.Start();
            LOG_INF("Autosaved project\n");
        }

        FramebufferSpecification spec = FrameBuffer->GetSpecification();
        bool framebufferRefletsScreenSize = ViewportSize.X > 0.0f && ViewportSize.Y > 0.0f && (spec.Width != ViewportSize.X || spec.Height != ViewportSize.Y);
        if (framebufferRefletsScreenSize || Application::Get()->GetWindow()->WasMinimized() || Application::Get()->GetWindow()->IsMaximized())
        {
            FrameBuffer->Resize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
            EditorCamera.OnResize(ViewportSize.X, ViewportSize.Y);
        }

        RendererConfig::Clear();
        FrameBuffer->Bind();
        RendererConfig::Clear();

        if(PlayStatus == SceneStatus::NOT_STARTED)
        {
            ActiveScene->OnEditingUpdate(ts, EditorCamera.GetCamera());
            if(ViewportFocused && ViewportHovered)
                EditorCamera.OnUpdate(ts);
        }
        else
        {
            if(ActiveScene->OnUpdate(ts, PlayStatus))
            {
                PlayStatus = SceneStatus::NOT_STARTED;
            }
        }

        FrameBuffer->Unbind();   
    }

    void NebulaStudioLayer::OnImGuiRender()
    {


        ImGui::Begin("Controls");
        
        ImGui::PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3, 0.3, 0.3, 0.5});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0, 0, 0, 0});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 1, 1 });

        bool greyOutPlay = PlayStatus != SceneStatus::NOT_STARTED && PlayStatus != SceneStatus::PAUSED;
        bool greyOutPause = PlayStatus == SceneStatus::NOT_STARTED || PlayStatus == SceneStatus::PAUSED;
        bool greyOutStop = PlayStatus == SceneStatus::NOT_STARTED;

        if (greyOutPlay)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        if(ImGui::ImageButton((ImTextureID)Tex_PlayButton->GetRendererID(), { 30, 30 }))
        {
            if (!ActiveScene->GetPrimaryCamera().IsValid())
            {
                LOG_ERR("Could not play! There was no camera in the scene.\n");
            }
            else
            {
                LOG_TMI("Play pressed\n");
                if (PlayStatus == SceneStatus::NOT_STARTED)
                {
                    ActiveScene->OnPlay();
                }
                PlayStatus = SceneStatus::PLAYING;
            }
        }

        if (greyOutPlay)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if (greyOutPause)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::SameLine();
        if(ImGui::ImageButton((ImTextureID)Tex_PauseButton->GetRendererID(), { 30, 30 }))
        {
            LOG_TMI("Pause pressed\n");
            if (PlayStatus == SceneStatus::PLAYING)
            {
                PlayStatus = SceneStatus::PAUSED;
            }
        }

        if (greyOutPause)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if (greyOutStop)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::SameLine();
        if(ImGui::ImageButton((ImTextureID)Tex_StopButton->GetRendererID(), { 30, 30 }))
        {
            LOG_TMI("Stop pressed\n");
            PlayStatus = SceneStatus::NOT_STARTED;
            ActiveScene->OnStop();
        }

        if (greyOutStop)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        ImGui::End();

        OnImGuiMenuBar();
        SceneHierarchy.OnImGuiRender();

        bool t = true;
        Log.Update(t);

        ImGui::Begin("Stats");
        
        Renderer2DStatistics stats = Renderer2D::GetStats();
        
        ImGui::Text("FPS            : %f", Application::Get()->GetFPS());
        ImGui::Text("UPS            : %f", Application::Get()->GetUPS());
        ImGui::Separator();
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

        uint32_t textureID = FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ ViewportSize.X, ViewportSize.Y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void NebulaStudioLayer::OnEvent(Event& e)
    {
        if (PlayStatus == SceneStatus::NOT_STARTED && ViewportFocused && ViewportHovered)
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

        if (shift && Input::IsKeyPressed(KeyCode::F5))
        {
            Renderer::ReloadShaders();
        }

        if (Input::IsKeyPressed(KeyCode::Delete))
        {
            SceneHierarchy.RemoveSelection();
        }

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
                case(KeyCode::C):
                {
                    CopyEntity();
                    break;
                }
                case(KeyCode::V):
                {
                    PasteEntity();
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
                if (ImGui::MenuItem("New/Open Project"))
                {
                    ProjectSelection();
                }
                if (ImGui::MenuItem("Open Recent Project"))
                {
                    OpenRecentProject();
                }
                if (ImGui::MenuItem("Save Project"))
                {
                    SaveProject(CurrentProject);
                }


                ImGui::Separator();
                if(ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
                {
                    OpenScene();
                }

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

                ImGui::Separator();
                // ImGui::Text("System");
                if (ImGui::MenuItem("Editor Preferences"))
                {
                    //TODO: Editor Preferences menu popup
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

    void NebulaStudioLayer::InitProject()
    {
        ActiveScene = CreateRef<Nebula::Scene>();
        if (!CurrentProject.LastSceneOpened.empty())
        {
            SceneSerializer ser(ActiveScene);
            ser.DeserializeTxt(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
            ActiveScene->SetFilePath(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
        }

        Autosave.Start();
        
        PlayStatus = SceneStatus::NOT_STARTED;
        EditorCamera = OrthographicCameraController(ViewportSize.X / ViewportSize.X);

        SceneHierarchy.SetContext(ActiveScene);
    }
    
    void NebulaStudioLayer::OpenRecentProject()
    {
        ImGui::MenuItem("This is a menu bar item");
    }

    void NebulaStudioLayer::ProjectSelection()
    {
        *App_SelectNewProject = true;
    }

    void NebulaStudioLayer::SaveProject(StudioProject::Project proj)
    {
        StudioProject::SaveProjectFile(proj);
        LOG_INF("Project saved\n");
    }
    
    void NebulaStudioLayer::OpenProject(const std::string& projPath)
    {
        //Mount VFS
        if (VFS::Exists(projPath, true))
        {
            CurrentProject = StudioProject::LoadProjectFile(projPath);
            
        }
        else
        {
            CurrentProject = StudioProject::CreateProjectFile(projPath);
        }

        if (VFS::Exists(CurrentProject.LastFileSystemMount, true))
        {
            VFS::Mount(CurrentProject.LastFileSystemMount);
        }
        else
        {
            std::string dir = std::string(CurrentProject.AbsolutePath);
            dir = dir.substr(0, dir.find_last_of("/") + 1);

            
            if (VFS::Exists(dir, true))
            {
                CurrentProject.LastFileSystemMount = dir;
                VFS::Mount(CurrentProject.LastFileSystemMount);
            }
        }

        InitProject();
    }

    void SaveProject(StudioProject::Project proj);

    void NebulaStudioLayer::NewScene()
    {
        ActiveScene = CreateRef<Nebula::Scene>();
        ActiveScene->OnViewportResize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
        SceneHierarchy.SetContext(ActiveScene);

        LOG_INF("New scene created.\n");
        SceneHierarchy.ClearSelection();

        VFS::Unmount();
    }
    
    void NebulaStudioLayer::SaveScene()
    {
        std::string filePath = ActiveScene->GetFilePath();

        if (filePath.empty())
        {
            SaveSceneAs();
            return;
        }
        
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
    
    void NebulaStudioLayer::OpenScene(std::string scenePath)
    {
        std::string filePath;

        {
            if (filePath.empty())
                std::string filePath = Nebula::FileDialogs::OpenFile("nst");
            else
                filePath = scenePath;
        }
            
        if (!filePath.empty())
        {
            
            std::string relPath = VFS::Path(filePath);

            ActiveScene = CreateRef<Nebula::Scene>();
            ActiveScene->SetFilePath(relPath);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
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

    
    void NebulaStudioLayer::CopyEntity()
    {
        if (!SceneHierarchy.HasSelection())
        {
            LOG_ERR("Could not copy, no selection\n");
            return;
        }
        else if (!SceneHierarchy.GetSelection().IsValid())
        {
            LOG_ERR("Could not copy, invalid selection\n");
            return;
        }

        Clipboard = SceneHierarchy.GetSelection();
        ClipboardFull = true;
        LOG_INF("Copied entity: (%d) %s\n", Clipboard.GetID(), Clipboard.GetComponent<TagComponent>().Tag.c_str());
    }

    void NebulaStudioLayer::PasteEntity()
    {
        if (!ClipboardFull)
        {
            LOG_WRN("Could not paste entity. Reason: Nothing to paste!\n");
            return;
        }
        else if (!Clipboard.IsValid())
        {
            LOG_WRN("Could not paste entity. Reason: Clipboard was not valid!\n");
            return;
        }
        std::string tag = Clipboard.GetComponent<TagComponent>().Tag;
        Entity newEntity = ActiveScene->CreateEntity(tag); 

        if (Clipboard.HasComponent<TransformComponent>())
        {
            auto tc = Clipboard.GetComponent<TransformComponent>();
            auto& tcNew = newEntity.GetComponent<TransformComponent>();
            tcNew = TransformComponent(tc);
        }

        if (Clipboard.HasComponent<SpriteRendererComponent>())
        {
            auto src = Clipboard.GetComponent<SpriteRendererComponent>();
            auto& srcNew = newEntity.AddComponent<SpriteRendererComponent>(src);
            srcNew = SpriteRendererComponent(src);
        }

        if (Clipboard.HasComponent<CameraComponent>())
        {
            auto cc = Clipboard.GetComponent<CameraComponent>();
            auto& ccNew = newEntity.AddComponent<CameraComponent>(cc);
            ccNew = CameraComponent(cc);
        }

        if (Clipboard.HasComponent<NativeScriptComponent>())
        {
            auto nsc = Clipboard.GetComponent<NativeScriptComponent>();
            auto& nscNew = newEntity.AddComponent<NativeScriptComponent>(nsc);
            nscNew = NativeScriptComponent(nsc);
        }

        if (Clipboard.HasComponent<ParentEntityComponent>())
        {
            auto pec = Clipboard.GetComponent<ParentEntityComponent>();
            auto& pecNew = newEntity.AddComponent<ParentEntityComponent>(pec);
            pecNew = ParentEntityComponent(pec);
        }

        if (SceneHierarchy.GetSelection().IsValid())
        {
            if (!SceneHierarchy.GetSelection().HasComponent<ParentEntityComponent>())
            {
                SceneHierarchy.GetSelection().AddComponent<ParentEntityComponent>();
            } 
            auto& parentComp = SceneHierarchy.GetSelection().GetComponent<ParentEntityComponent>();

            parentComp.children.push_back(newEntity);
        }
        else if (Clipboard.HasComponent<RootEntityComponent>())
        {
            auto root = Clipboard.GetComponent<RootEntityComponent>();
            auto& rootNew = newEntity.AddComponent<RootEntityComponent>(root);
            rootNew = RootEntityComponent(root);
        }

        LOG_INF("Entity pasted to active scene\n");
    }

}
