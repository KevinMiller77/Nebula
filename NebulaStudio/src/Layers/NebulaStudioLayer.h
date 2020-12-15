#pragma once
#include <Nebula.h>

#include "../Base/NebulaStudioProject.h" 

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/LogPanel.h"

namespace Nebula
{
    #define SEC_TO_MINUTE 60.f
    #define AUTOSAVE_INTERVAL (SEC_TO_MINUTE * 2.5f)


    class NebulaStudioLayer : public Layer
    {
    public:

        NebulaStudioLayer(std::string startProjectFile, Ref<bool> needNewProject)
            : EditorCamera(OrthographicCameraController(ViewportSize.X / ViewportSize.Y)), ProjFileInput(startProjectFile), App_SelectNewProject(needNewProject)
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

        void InitInternalTextures();

        virtual void OnAttach() override;
        virtual void OnUpdate(float ts) override;
        void OnImGuiMenuBar();
        virtual void OnImGuiRender() override;
        
        virtual void OnEvent(Event& e) override;
        bool OnKeyPressed(KeyPressedEvent& e);

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


        void CopyEntity();
        bool IsClipboardFull() { return ClipboardFull; }
        void PasteEntity();

    public:
        Ref<Scene> ActiveScene;
        Entity CameraEntity;

        static Vec4f clearColor;
        Ref<Framebuffer> FrameBuffer;
        bool ViewportFocused = false, ViewportHovered = false;
        Vec2f ViewportSize = {1600.0f, 900.0f};

        OrthographicCameraController EditorCamera;

    private:
		Timer Autosave = Timer();
        std::string ProjFileInput = std::string();
        std::string NewProjFileInput = std::string();

        Entity Clipboard;
        bool ClipboardFull = false;

        // Must be set on creation, it is passed through from the application as a hook
        Ref<bool> App_SelectNewProject = nullptr;

        SceneHierarchyPanel SceneHierarchy;
        LogPanel Log;
        // ImGui::FileBrowser FileBrowser;
        StudioProject::Project CurrentProject;

        // Engine Needed Textures, should be set in InitInternalTextures
        Ref<Texture2D> Tex_PlayButton;
        Ref<Texture2D> Tex_PauseButton;
        Ref<Texture2D> Tex_StopButton;
    };

}