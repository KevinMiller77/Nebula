#pragma once
#include <Nebula.h>

#include "Panels/FileBrowserPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/LogPanel.h"

namespace Nebula
{
    enum class SceneStatus
    {
        NONE = 0,
        NOT_STARTED,
        PLAYING,
        PAUSED,
    };

    class NebulaStudioLayer : public Layer
    {
    public:
        Ref<Scene> ActiveScene;
        Entity CameraEntity;

        static Vec4f clearColor;
        Framebuffer* FrameBuffer;
        bool ViewportFocused = false, ViewportHovered = false;
        Vec2f ViewportSize = {1280.0f, 720.0f};

        Shader* shader = nullptr;
        TextureLibrary textures;
        OrthographicCameraController EditorCamera;

        NebulaStudioLayer()
            : EditorCamera(OrthographicCameraController(ViewportSize.x / ViewportSize.y))
        {

        }
        ~NebulaStudioLayer() = default;

        SceneStatus PlayStatus = SceneStatus::NOT_STARTED;

        virtual void OnAttach() override;
        virtual void OnUpdate(float ts) override;
        void OnImGuiMenuBar();
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& e) override;

        SceneHierarchyPanel SceneHierarchyPanel;
        LogPanel Log;
        ImGui::FileBrowser FileBrowser;
    };

}