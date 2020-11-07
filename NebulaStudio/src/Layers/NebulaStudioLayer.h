#pragma once
#include <Nebula.h>

#include "../Base/NebulaStudioProject.h" 

#include "Panels/FileBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/LogPanel.h"

namespace Nebula
{
    #define AUTOSAVE_INTERVAL 5.0f


    class NebulaStudioLayer : public Layer
    {
    public:
        Ref<Scene> ActiveScene;
        Entity CameraEntity;

        static Vec4f clearColor;
        Framebuffer* FrameBuffer;
        bool ViewportFocused = false, ViewportHovered = false;
        Vec2f ViewportSize = {1600.0f, 900.0f};

        Shader* shader = nullptr;
        TextureLibrary textures;
        OrthographicCameraController EditorCamera;

        NebulaStudioLayer(std::string startProjectFile)
            : EditorCamera(OrthographicCameraController(ViewportSize.X / ViewportSize.Y)), StartProjFileInput(startProjectFile)
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

        void NewScene();
        void SaveScene();
        void SaveSceneAs();
        void OpenScene();
    private:
		Timer Autosave = Timer();
        std::string StartProjFileInput = std::string();

        SceneHierarchyPanel SceneHierarchy;
        LogPanel Log;
        ImGui::FileBrowser FileBrowser;
        StudioProject::Project CurrentProject;
    };

}