#pragma once
#include <Nebula.h>

#include "../Base/NebulaStudioProject.h" 
#include "../StudioApp.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/LogPanel.h"

#include <ImGuizmo.h>

namespace Nebula
{
    #define SEC_TO_MINUTE 60.f
    #define AUTOSAVE_INTERVAL (SEC_TO_MINUTE * 2.5f)


    class NebulaStudioLayer : public Layer
    {
    public:

        NebulaStudioLayer(std::string startProjectFile, Ref<bool> needNewProject)
            : m_EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f), ProjFileInput(startProjectFile), App_SelectNewProject(needNewProject)
        {
        }

        ~NebulaStudioLayer()
        {
            if (CurrentProject.loaded)
            {
                StudioProject::SaveProjectFile(CurrentProject);
            }
        }

        SceneStatus PlayStatus = SceneStatus::NOT_STARTED;

        virtual void OnAttach() override;
        virtual void OnUpdate(float ts) override;
        void OnImGuiMenuBar();
        virtual void OnImGuiRender() override;
        
        virtual void OnEvent(Event& e) override;
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void InitProject();
        void ProjectSelection();
        
        void OpenProject(const std::string& projPath);
        void SaveProject(StudioProject::Project proj);


        void NewScene();
        void SaveScene();
        void SaveSceneAs();
        // Pass in a path to the scene to open or
        void OpenScene(std::string scenePath = std::string());
        
        // Menubar Elements
        void OpenRecentProject();

        Entity GetSelectionFromSHP();

        void CopyEntity(bool cut = false);
        void CutEntity() { CopyEntity(true); }
        bool IsClipboardFull() { return ClipboardStatus != EMPTY; }
        void PasteEntity();
        void DeleteEntity();

        // Toggle colored axis lines
        void ToggleGrid();

    public:
        Ref<Scene> ActiveScene;
        Entity CameraEntity;
        Entity AxisGrid, AxisGridX, AxisGridY, AxisGridZ; 

        void SetupAxisGrid();

        Ref<Framebuffer> FrameBuffer;
        bool ViewportFocused = false, ViewportHovered = false;
        Vec2f ViewportSize = {1600.0f, 900.0f};
        EditorCamera m_EditorCamera;
        
    private:
		Timer Autosave = Timer();
        std::string ProjFileInput = std::string();
        std::string NewProjFileInput = std::string();

        Vec2f m_ViewportBounds[2];
        int m_HoveringEntity = -1;
        bool m_HoveringViewport = false;

        Entity Clipboard;

        enum ClipboardStat
        {
            EMPTY,
            COPY,
            CUT
        } ClipboardStatus = EMPTY;

        // Must be set on creation, it is passed through from the application as a hook
        Ref<bool> App_SelectNewProject = nullptr;

        SceneHierarchyPanel SceneHierarchy;
        LogPanel Log;
        // ImGui::FileBrowser FileBrowser;
        StudioProject::Project CurrentProject;

        bool m_RenderImGuizmo = true;
        ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE; 
    };

}