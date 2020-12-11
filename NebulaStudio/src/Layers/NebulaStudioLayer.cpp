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
            dir = dir.substr(0, dir.find_last_of("/") + 1);

            
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

        Autosave.Start();

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1600;
        fbSpec.Height = 900;
        FrameBuffer = Framebuffer::Create(fbSpec);

        PlayStatus = SceneStatus::NOT_STARTED;
        EditorCamera = OrthographicCameraController(ViewportSize.X / ViewportSize.X);

        SceneHierarchy.SetContext(ActiveScene);
        SceneHierarchy.SetTextureLib(&textures);

        FileBrowser.SetTitle("Project Browser");
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
            StudioProject::SaveProjectFile(CurrentProject);
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
        
        Renderer2DStatistics stats = Renderer2D::GetStats();
        
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
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ ViewportSize.X, ViewportSize.Y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        
        
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
    
    void NebulaStudioLayer::OpenScene()
    {
        std::string filePath = Nebula::FileDialogs::OpenFile("nst");
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

        if (Clipboard.HasComponent<RootEntityComponent>())
        {
            auto root = Clipboard.GetComponent<RootEntityComponent>();
            auto& rootNew = newEntity.AddComponent<RootEntityComponent>(root);
            rootNew = RootEntityComponent(root);
        }

        LOG_INF("Entity pasted to active scene\n");
    }

}
