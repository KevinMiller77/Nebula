#include "NebulaStudioLayer.h"

namespace Nebula
{
    Vec2f BoxPos = Vec2f(0.0, 0.0);
    bool drawMesh = false;
    Ref<Mesh> s_Mesh1;

    // Happens every time you open a project

    void NebulaStudioLayer::OnAttach()
    {

        FramebufferSpecification fbSpec;
        fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth};
        fbSpec.Width = 1600;
        fbSpec.Height = 900;
        fbSpec.Samples = 1;
        fbSpec.ClearColor = {0.1f, 0.1f, 0.1f, 1.0f};
        RendererConfig::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderPassSpecification rpSpec;
        rpSpec.TargetFramebuffer = Framebuffer::Create(fbSpec);

        m_RenderPassSpec = rpSpec;

        // Get a scene from the project input
        OpenProject(ProjFileInput);

        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
    }

    float tsls = 0.0f;
    void NebulaStudioLayer::OnUpdate(float ts)
    {
        if (IsClipboardFull() && !Clipboard.IsValid())
        {
            ClipboardStatus = EMPTY;
            Clipboard = Entity();
        }

        if (Autosave.GetTimePassed() >= AUTOSAVE_INTERVAL)
        {
            SaveProject(CurrentProject);
            Autosave.Start();
            LOG_INF("Autosaved project\n");
        }

        FramebufferSpecification spec = m_RenderPassSpec.TargetFramebuffer->GetSpecification();
        bool framebufferRefletsScreenSize = ViewportSize.X > 0.0f && ViewportSize.Y > 0.0f && (spec.Width != ViewportSize.X || spec.Height != ViewportSize.Y);
        if (framebufferRefletsScreenSize || Application::Get()->GetWindow()->WasMinimized() || Application::Get()->GetWindow()->IsMaximized())
        {
            m_RenderPassSpec.TargetFramebuffer->Resize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
            m_EditorCamera.SetViewportSize(ViewportSize.X, ViewportSize.Y);
        }

        m_RenderPassSpec.TargetFramebuffer->Bind();
        RendererConfig::SetAlphaBlend(true);
        RendererConfig::Clear();

        if(PlayStatus == SceneStatus::NOT_STARTED)
        {
            // auto v = ActiveScene->GetView<AudioSourceComponent>();
            // for (auto entityHandle : v) {
            //     Entity e = { entityHandle, ActiveScene.get() };
            //     auto& asc = e.GetComponent<AudioSourceComponent>();

            //     if (asc.Source->IsPlaying()) {
            //         asc.Source->Stop();
            //     }
            // }

            m_RenderPassSpec.TargetFramebuffer->ClearTextureAttachment(1, -1);

            m_EditorCamera.OnUpdate(ts, ViewportFocused || ViewportHovered);
            ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

            if (s_Mesh1) {
                Ref<RenderPass> pass = RenderPass::Create(m_RenderPassSpec);
                Renderer::BeginRenderPass(pass, false);
                if(s_Mesh1->IsLoaded()) {
                    Renderer::SubmitMesh(s_Mesh1, Mat4f(1.0f));
                }
                Renderer::EndRenderPass();
            }

            Vec2f pos = {ImGui::GetMousePos().x, ImGui::GetMousePos().y};
            pos.X -= m_ViewportBounds[0].X; 
            pos.Y -= m_ViewportBounds[0].Y;
            Vec2f viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
            pos.Y = viewportSize.Y - pos.Y;
            int mouseX = (int)pos.X;
            int mouseY = (int)pos.Y;



            if (ViewportFocused && ViewportHovered) {
                m_HoveringEntity = m_RenderPassSpec.TargetFramebuffer->ReadPixel(1, mouseX, mouseY);
            }
            else {
                m_HoveringEntity = -1;
            }

        }
        else
        {
            ActiveScene->OnUpdateRuntime(ts);
        }

        m_RenderPassSpec.TargetFramebuffer->Unbind();   
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
            LOG_TMI("Play pressed\n");
            if (PlayStatus == SceneStatus::NOT_STARTED)
            {
                ActiveScene->OnPlay();
            }
            PlayStatus = SceneStatus::PLAYING;
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
        ImGui::Text("Line Count     : %d", stats.LineCount);
        ImGui::Text("Vertex Count   : %d ", stats.GetTotalVertexCount());    
        ImGui::Text("Index Count    : %d", stats.GetTotalIndexCount());

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        Vec2f viewportOffset = {ImGui::GetCursorPos().x, ImGui::GetCursorPos().y};

        ViewportFocused = ImGui::IsWindowFocused();
        ViewportHovered = ImGui::IsWindowHovered();
        Application::Get()->GetImGuiLayer()->BlockEvents(!ViewportFocused || !ViewportHovered);


        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        if (m_EditorCamera.GetViewportSize() != ViewportSize)
        {
            m_EditorCamera.SetViewportSize(ViewportSize.X, ViewportSize.Y);
        }

        uint32_t textureID = m_RenderPassSpec.TargetFramebuffer->GetColorAttachmentsRendererID()[0];
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ ViewportSize.X, ViewportSize.Y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        Vec2f windowSize = {ImGui::GetWindowSize().x, ImGui::GetWindowSize().y};

        Vec2f minBound = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};
        minBound.X += viewportOffset.X; 
        minBound.Y += viewportOffset.Y; 
        
        Vec2f maxBound = {minBound.X + windowSize.X, minBound.Y + windowSize.Y};

        m_ViewportBounds[0] = minBound;
        m_ViewportBounds[1] = maxBound;

        Entity selectedEntity = SceneHierarchy.GetSelection();

        // Check if we should draw gizmos
        bool shouldDrawImGuizmo = PlayStatus == SceneStatus::NOT_STARTED;
        shouldDrawImGuizmo = shouldDrawImGuizmo && m_RenderImGuizmo;
        shouldDrawImGuizmo = shouldDrawImGuizmo && selectedEntity.IsValid();

        // Only draw if entity has sprite component AND is not hidden
        if (shouldDrawImGuizmo) {
            bool shouldDrawSprite = selectedEntity.HasComponent<SpriteRendererComponent>() ? !selectedEntity.GetComponent<SpriteRendererComponent>().Hidden : false;
            bool shouldDrawMesh = selectedEntity.HasComponent<MeshComponent>() ? selectedEntity.GetComponent<MeshComponent>().Mesh && selectedEntity.GetComponent<MeshComponent>().Mesh->IsLoaded() : false;
            shouldDrawImGuizmo = shouldDrawImGuizmo && (shouldDrawMesh || shouldDrawSprite);
        }

        if (shouldDrawImGuizmo) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            Vec2f windowSize = {ImGui::GetWindowWidth(), ImGui::GetWindowHeight()}; 
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowSize.X, windowSize.Y);

            Mat4f viewMat = m_EditorCamera.GetViewMatrix();
            viewMat = Mat4f::AsRowMajor(viewMat);

            Mat4f projMat = m_EditorCamera.GetProjection();
            projMat = Mat4f::AsRowMajor(projMat);

            auto& selectedTC = selectedEntity.GetComponent<TransformComponent>();
            Mat4f entityTransform = selectedTC.GetTransformation();
            entityTransform = Mat4f::AsRowMajor(entityTransform);
            
            ImGuizmo::Manipulate(viewMat.elements, projMat.elements, m_CurrentGizmoOperation, ImGuizmo::MODE::LOCAL, entityTransform.elements);

            if(ImGuizmo::IsUsing()) {
                Vec3f t(0.0f), r(0.0f), s(0.0f); 
                DecomposeTransform(entityTransform, t, r, s);

                selectedTC.Translation = t;
                selectedTC.Rotation = r;
                selectedTC.Scale = s;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void NebulaStudioLayer::OnEvent(Event& e)
    {
        if (PlayStatus == SceneStatus::NOT_STARTED && ViewportHovered)
        {
            m_EditorCamera.OnEvent(e);
        }

        EventDispatcher Dispatch(e);
        Dispatch.Dispatch<KeyPressedEvent>(NEB_BIND_EVENT_FN(NebulaStudioLayer::OnKeyPressed));
        Dispatch.Dispatch<MouseButtonPressedEvent>(NEB_BIND_EVENT_FN(NebulaStudioLayer::OnMouseButtonPressed));
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

        // CTRL only
        if (ctrl && !shift)
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
                case(KeyCode::X):
                {
                    CutEntity();
                    break;
                }
                case(KeyCode::V):
                {
                    PasteEntity();
                    break;
                }

                default:
                    break;
            }
        }
        // SHIFT only
        else if (!ctrl && shift)
        {
            switch((KeyCode)e.GetKeyCode())
            {
                case(KeyCode::H):
                {
                    Entity sel = SceneHierarchy.GetSelection(); 
                    if(sel.IsValid())
                    {
                        if (sel.HasComponent<SpriteRendererComponent>())
                        {
                            auto& src = sel.GetComponent<SpriteRendererComponent>();
                            src.Hidden = !src.Hidden;
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }
        // Both CTRL and SHIFT
        else if (ctrl && shift)
        {
            switch ((KeyCode)e.GetKeyCode())
            {
                case(KeyCode::A):
                {
                    // TODO: Toggle Axis color
                    // ToggleAxis();
                    break;
                }
                case(KeyCode::G):
                {
                    // TODO: Toggle Grid 
                    // Also make the grid out of lines not sprites
                    // Also Also, make sure the grid changes with the distance of the editor camera focal point
                    ToggleGrid();
                    break;
                }

                default:
                    break;
            }
        }
        // Simple key press
        else
        {
            switch ((KeyCode)e.GetKeyCode())
            {
                
                case(KeyCode::D1): {
                    m_RenderImGuizmo = false;
                    break;
                }
                case(KeyCode::D2): {
                    m_RenderImGuizmo = true;
                    m_CurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                    break;
                }
                case(KeyCode::D3): {
                    m_RenderImGuizmo = true;
                    m_CurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                    break;
                }
                case(KeyCode::D4): {
                    m_CurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;                    
                    m_RenderImGuizmo = true;

                    break;
                }

                default:
                    break;
            }
        }

        return false;
    }

    
    bool NebulaStudioLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        if (ImGuizmo::IsOver() || !ViewportHovered) {
            return true;
        }

        switch (e.getButton()) {
            case(MouseCode::ButtonLeft): {
                if (m_HoveringEntity == -1) {
                    SceneHierarchy.ClearSelection();
                } else {
                    Entity startEntity = SceneHierarchy.GetSelection();

                    LOG_DBG("Setting selection: %d\n", m_HoveringEntity);
                    SceneHierarchy.SetSelection({(entt::entity)m_HoveringEntity, ActiveScene.get()});

                    // If selection changes, make sure gizmo is running
                    if (SceneHierarchy.GetSelection() != startEntity) {
                        m_CurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                        m_RenderImGuizmo = true;
                    }
                }
                break;
            }
            default: {
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
            if (ImGui::BeginMenu("Edit"))
            {
                bool haveSelection = GetSelectionFromSHP().IsValid(); 
                if (!haveSelection)
                {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }

                if (ImGui::MenuItem("Copy Entity", "Ctrl+C"))
                {
                    CopyEntity();
                }
                if (ImGui::MenuItem("Cut Entity", "Ctrl+X"))
                {
                    CutEntity();   
                }

                if(!haveSelection)
                {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }

                if (ClipboardStatus == EMPTY)
                {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }

                if (ImGui::MenuItem("Paste Entity", "Ctrl+V"))
                {
                    PasteEntity();
                }
                
                if (ClipboardStatus == EMPTY)
                {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
                
                ImGui::Separator();
                if (ImGui::MenuItem("Delete Entity", "Del"))
                {
                    DeleteEntity();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About"))
            {
                ImGui::Text("Nebula Studio");
                ImGui::Text("Ver 0.6.3");
                ImGui::Text("");
                ImGui::Text("Main Developer:");
                ImGui::Text("Kevin Miller");
                ImGui::Text("");
                ImGui::Text("Graphic Designer:");
                ImGui::Text("Evan Hiltik");

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

    }

    void NebulaStudioLayer::InitProject()
    {
        ActiveScene = CreateRef<Nebula::Scene>(m_RenderPassSpec);
        if (!CurrentProject.LastSceneOpened.empty())
        {
            SceneSerializer ser(ActiveScene);
            ser.DeserializeTxt(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
            ActiveScene->SetFilePath(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
        }

        Autosave.Start();
        
        PlayStatus = SceneStatus::NOT_STARTED;
        m_EditorCamera = EditorCamera(45.0f, ViewportSize.X / ViewportSize.Y, 0.1f, 1000.0f);

        SetupAxisGrid();

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
        AddRecentProject(CurrentProject.AbsolutePath);

        if (VFS::Exists(CurrentProject.LastFileSystemMount, true))
        {
            VFS::Mount(CurrentProject.LastFileSystemMount);
            if (VFS::Exists(CurrentProject.LastSceneOpened))
            {
                OpenScene(VFS::AbsolutePath(CurrentProject.LastSceneOpened));
            }
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
        ActiveScene = CreateRef<Nebula::Scene>(m_RenderPassSpec);
        ActiveScene->OnViewportResize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
        SceneHierarchy.SetContext(ActiveScene);


        LOG_INF("New scene created.\n");
        SceneHierarchy.ClearSelection();

        SetupAxisGrid();
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

        if (scenePath.empty())
        {
            filePath = Nebula::FileDialogs::OpenFile("nst");
        }
        else
        {
            filePath = scenePath;
        }
                    
        if (!filePath.empty())
        {
            
            std::string relPath = VFS::Path(filePath);

            ActiveScene = CreateRef<Nebula::Scene>(m_RenderPassSpec);
            ActiveScene->SetFilePath(relPath);
            ActiveScene->OnViewportResize((uint32_t)ViewportSize.X, (uint32_t)ViewportSize.Y);
            SceneHierarchy.SetContext(ActiveScene);

            //Nebula::SceneSerializer serializer(ActiveScene);
            //serializer.DeserializeTxt(filePath);

            CurrentProject.LastSceneOpened = relPath;

            SetupAxisGrid();

            LOG_INF("Opened scene!\n");
            return;
        }
        LOG_INF("Open scene cancelled!\n");
        SceneHierarchy.ClearSelection();
    }

    Entity NebulaStudioLayer::GetSelectionFromSHP()
    {
        if (!SceneHierarchy.HasSelection())
        {
            // LOG_ERR("Could not get selection from SHP, no selection\n");
            return {entt::null, nullptr};
        }
        else if (!SceneHierarchy.GetSelection().IsValid())
        {
            // LOG_ERR("Could not get selection from SHP, invalid selection\n");
            return {entt::null, nullptr};
        }

        return SceneHierarchy.GetSelection();
    }

    
    void NebulaStudioLayer::CopyEntity(bool cut)
    {
        Entity e = GetSelectionFromSHP();
        if (!e.IsValid())
        {
            LOG_INF("Copy failed.\n");
            return;
        }

        Clipboard = e;
        ClipboardStatus = cut ? CUT : COPY;

        LOG_INF("Copied entity: (%d) %s\n", Clipboard.GetID(), Clipboard.GetComponent<TagComponent>().Tag.c_str());
    }

    void NebulaStudioLayer::PasteEntity()
    {
        if (!IsClipboardFull())
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
        // if (Clipboard.HasComponent<AudioListenerComponent>())
        // {
        //     auto pec = Clipboard.GetComponent<AudioListenerComponent>();
        //     auto& pecNew = newEntity.AddComponent<AudioListenerComponent>(pec);
        //     pecNew = AudioListenerComponent(pec);
        // }
        // if (Clipboard.HasComponent<AudioSourceComponent>())
        // {
        //     auto pec = Clipboard.GetComponent<AudioSourceComponent>();
        //     auto& pecNew = newEntity.AddComponent<AudioSourceComponent>(pec);
        //     pecNew.Source = AudioSource::LoadFromFile(pec.Source->GetFilePath(), false);
        // }

        bool pastingAsChild = SceneHierarchy.GetSelection().IsValid(); 
        if (pastingAsChild)
        {
            if (!SceneHierarchy.GetSelection().HasComponent<ParentEntityComponent>())
            {
                SceneHierarchy.GetSelection().AddComponent<ParentEntityComponent>();
            } 
            auto& parentComp = SceneHierarchy.GetSelection().GetComponent<ParentEntityComponent>();

            parentComp.children.push_back(newEntity);
        }

        if (Clipboard.HasComponent<RootEntityComponent>())
        {
            auto root = Clipboard.GetComponent<RootEntityComponent>();
            auto& rootNew = newEntity.AddComponent<RootEntityComponent>(root);
            rootNew = RootEntityComponent(root);
        }


        if (ClipboardStatus == CUT)
        {
            ActiveScene->RemoveEntity(Clipboard);
            ClipboardStatus == EMPTY;
        }

        LOG_INF("Entity pasted to active scene\n");
    }

    void NebulaStudioLayer::DeleteEntity()
    {
        SceneHierarchy.RemoveSelection();
    }

    void NebulaStudioLayer::SetupAxisGrid()
    {
        if (!ActiveScene)
            return;
        
        AxisGrid = ActiveScene->CreateEntity("Axis Grid Base");

        auto& src = AxisGrid.AddComponent<SpriteRendererComponent>();
        src.Type = SpriteRendererComponent::RenderType::NONE;

        // Ensure the SHP will never see this entity (or it's children)
        auto& root = AxisGrid.AddComponent<RootEntityComponent>();
        root.VisibleOutsideRenderer = false;

        auto& parent = AxisGrid.AddComponent<ParentEntityComponent>();


        AxisGridX = ActiveScene->CreateEntity("Axis Grid X");
        // Get tranform comps
        auto& xTC = AxisGridX.GetComponent<TransformComponent>();
        xTC.LineCoords = { {-5000.0f, 0.0f, 0.0f }, {5000.0f, 0.0f, 0.0f } };

        // Make the sprite render comps
        auto& xSRC = AxisGridX.AddComponent<SpriteRendererComponent>();
        xSRC.Color = {0.5f, 0.0f, 0.0f, 0.75f};
        xSRC.Type = SpriteRendererComponent::RenderType::LINE;

        parent.children.push_back(AxisGridX);


        AxisGridY = ActiveScene->CreateEntity("Axis Grid Y");

        auto& yTC = AxisGridY.GetComponent<TransformComponent>();
        yTC.LineCoords = { {0.0f, -5000.0f, 0.0f }, {0.0f, 5000.0f, 0.0f } };

        auto& ySRC = AxisGridY.AddComponent<SpriteRendererComponent>();
        ySRC.Type = SpriteRendererComponent::RenderType::LINE;
        ySRC.Color = {0.0f, 0.5f, 0.0f, 0.75f};

        parent.children.push_back(AxisGridY);
        

        AxisGridZ = ActiveScene->CreateEntity("Axis Grid Z");

        auto& zTC = AxisGridZ.GetComponent<TransformComponent>();
        zTC.LineCoords = { {0.0f, 0.0f, -5000.0f }, {0.0f, 0.0f, 5000.0f } };

        auto& zSRC = AxisGridZ.AddComponent<SpriteRendererComponent>();
        zSRC.Type = SpriteRendererComponent::RenderType::LINE;
        zSRC.Color = {0.0f, 0.0f, 0.5f, 0.75f};

        parent.children.push_back(AxisGridZ);
    }

    void NebulaStudioLayer::ToggleGrid()
    {
        if (AxisGrid.IsValid())
        {
            auto& src = AxisGrid.GetComponent<SpriteRendererComponent>();
            src.Hidden = !src.Hidden;
            LOG_INF("Toggled axis grid\n");
        }
    }
}
